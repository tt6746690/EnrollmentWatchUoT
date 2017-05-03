
# coding: utf-8

# In[198]:

from os import path, makedirs
import re
import json

from urllib.request import urlopen
from urllib.parse import urlencode, urljoin
from urllib.error import HTTPError

import lxml.html as html
import cssselect

from timeit import default_timer as timer
from ast import literal_eval


TIMETABLE_API_URL = "https://timetable.iit.artsci.utoronto.ca/api/"
CUR_SESSION = "20169"


## Get course codes from Calendar / TimeTable 
## Subsequent method is called once with result saved 
## and does not require frequent updates

def get_course_code(subject):
    """ Parse calendar and extract course 
        code based on subject store the resulting
        list into file under ../resources

        String subject
            subject code, (i.e. CSC)
        rType String[]
            a list of subject code (i.e. CSC108)
    """
    course_codes = []
    
    s_lower = subject.lower()
    s_upper = subject.upper()

    url = "http://calendar.artsci.utoronto.ca/crs_{}.htm"                 .format(s_lower)
        
    regex = re.compile(r"^([A-Z]{3}[0-9]{3})(\w\d)$")

    with urllib.request.urlopen(url) as f:
        doc = html.fromstring(f.read().decode('utf-8'))
        elements = doc.cssselect("body > a")

        for e in elements:
            c = e.get('name')
            if c:
                code = regex.match(c)
                if code:
                    course_codes.append(code.group(1))
        
    return course_codes


def get_subject_list():
    """ Query time table /api/ogs 
        and return a list of sorted subject codes

        rType String[subject]
            (i.e. ['ABP', 'ACT', ..])
    """
    
    url = TIMETABLE_API_URL + "orgs"   
    subject_list = []
    
    with urllib.request.urlopen(url) as f:
        j = json.loads(f.read().decode('utf-8'))
        if j.get("orgs"):
            d = j["orgs"]
            for key, value in d.items():
                subject = re.findall(r"(\([A-Z]{3}\))+", value)
                if subject:
                    subject_list.extend([s[1:4] for s in subject])
    
    # remove duplicates
    l = sorted([x for x in set(subject_list)])
    
    if not os.path.isfile("../resources/subject_list"):
        with open("../resources/subject_list", 'w') as f:
            f.write(str(l))
                  
    return l 
    

def get_all_course_code():
    """ Parse calendar and extract course 
        code for every subject

        Rtype list[course code] 
            (i.e. ['ACT230', 'ACT240, ... ])
    """
    
    codes = set()
    
    no_cal_subject = set()
    
    for subject in get_subject_list():
        try:
            l = get_course_code(subject)
            codes.update(set(l))
        except HTTPError as e:
            if e.code == 404:
                no_cal_subject.add(subject)
            pass  

    s_list = sorted(codes)
    
    with open("../resources/course_codes", "w") as f:
        f.write(json.dumps(s_list))
    
    return s_list



## Parsing method to be called regularly

def store_course_json(url, dp):
    """ Process one json response from querying course
        info on timetable given url, return the 
        json file while storing it under dir path dp
        
        String url
            url to get json 
        String dp 
            directory to store json in
    """
    with urllib.request.urlopen(url) as res:
        
        buf = res.read()
        
        # path => resources/20169/CSC209
        with open(dp, 'wb') as f:
            f.write(buf)
            
        j = json.loads(buf.decode('utf-8'))
        return j
    
def store_course_json(course_codes, dp):
    """ Store json with 
        /api/session/courses?code=course_code
        for every codes in course_codes under 
        dir path dp
        
        String[] course_codes
            (i.e. ['ACT230', 'ACT240', ..])
        String dp 
            directory to store json in
    """
    
    # fetch json from timetable 
    base_url = TIMETABLE_API_URL + CUR_SESSION + "/courses?"
    
    for course in course_codes:
        params = urllib.parse.urlencode({"code": course})
        url = base_url + params
        
        inval_course = []
        
        try:
            c_json = get_course_json(url, 
                                     os.path.join(dp, course))
        except HTTPError as e:
            inval_course.append(course)
            
            print(course)
            pass

def update():
    """ Scrap TimeTable for course info 
    if the website is newly updated 
    """
                         



if __name__ == "__main__":
        
    course_codes = []
    with open("../resources/course_codes", "r") as f:
        course_codes = ast.literal_eval(f.read())
    
    # Scrap TimeTable website and store json
    JSON_STORAGE_PATH = "../resources/" + CUR_SESSION
    
    
    start = timer()
    store_course_json(course_codes, JSON_STORAGE_PATH)
    end = timer()
    print(end - start)      

    
    





