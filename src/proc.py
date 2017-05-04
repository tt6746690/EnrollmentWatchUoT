
# coding: utf-8

# In[73]:


from os import listdir, path
import json

# Holds the timestamp for current json file
CUR_EPOCH_TIME = 0

ARCHIVE_DIR = "../../tt_archive/201701/"
DATA_TABLE_DIR = "../resources/201701_proc/"

DATA_TABLE_HEADERS = [
        "courseCode",
        "section",
        "meetingId",
        "teachingMethod",
        "sectionNumber",
        "waitlist",
        "enrollmentCapacity",
        "actualEnrolment",
        "actualWaitlist",
        "epochTime"
    ]

                       
def proc_one(course, f):
    """ Extracts useful info from 
        one course, and append to f
        
        dict course
        file f
            f is writable
        rType list[meeting]
            meetings: [
                [
                    course_code: SLA208,
                    section: Y,
                    meetingId: 2193,
                    teachingMethod: LEC,
                    sectionNumber: 0501
                    waitlist: Y, 
                    enrollmentCapacity: 18,
                    actualEnrolment: 6,
                    actualWaitlist: 0
                ],
                ...
            ]
    """
    l = []

    ccode = course["code"][:6]
    sec = course["section"]  

    meetings = course["meetings"]
    for k, v in meetings.items(): 
        
        # cancelled courses does not have waitlist field
        if (v["cancel"] == "Cancelled"):
            raise Exception("{} {} cancelled".format(ccode, k))
            
        meeting = []
        meeting.append(ccode)
        meeting.append(sec)
        
        meeting.append(v["meetingId"])
        meeting.append(v["teachingMethod"])
        meeting.append(v["sectionNumber"])
        meeting.append(v["waitlist"])
        meeting.append(v["enrollmentCapacity"])
        meeting.append(v["actualEnrolment"])
        meeting.append(v["actualWaitlist"])
        
        meeting.append(str(CUR_EPOCH_TIME))
        l.append(meeting)
        
        # write current meeting to a line in data file
        f.write('\t'.join(meeting) + '\n')
    return l
            

    
def proc_entire(jp, dest):
    """ Extracts useful info  
        given json file path jp,
        store extracted table to 
        file dest
        
        str jp
            source json file path 
        str dest 
            destination data table storage path
        rType 
            list[meeting] 
    """
    mts = [];
    
    with open(jp, 'r') as f:
        with open(dest, 'w+') as df:
            
            # process each course in json file
            j = json.load(f)
            for k, v in j.items():
                try: 
                    mts.extend(proc_one(v, df))
                except KeyError as e:
                    print("proc {} dict KeyError".format(k))
                except Exception as e:
                    pass
    return mts
 
def proc_history():
    """Process every json file archived under 
        ARCHIVE_DIR
    """
    for f in listdir(ARCHIVE_DIR):
        if f != "latest.json":
            global CUR_EPOCH_TIME
            CUR_EPOCH_TIME = path.splitext(f)[0]

            proc_entire(path.join(ARCHIVE_DIR, f), 
                        path.join(DATA_TABLE_DIR, CUR_EPOCH_TIME))

def test_proc_one():
    with open("../resources/20169/SLA208") as f:
        with open(DATA_TABLE_FILE, 'w+') as df:
            df.write('\t'.join(DATA_TABLE_HEADERS) + '\n')

            j = json.load(f)
            for k, v in j.items():
                c = proc_one(v, df)
                print(c)

def cat_files_to(dest):
    """ Concatenate files in 
        /resources/201701_proc into 
        a file specified by path dest 
        
        str dest
    """
    with open(dest, 'w+') as outf:
        
        # write header
        outf.write('\t'.join(DATA_TABLE_HEADERS) + '\n')
        
        for f in listdir(DATA_TABLE_DIR):
            if f != "latest.json":
                with open(path.join(DATA_TABLE_DIR, f)) as inf:
                    outf.write(inf.read())
          

""" Processes the Time-Table-Archive json from 201701 
    by extracting enrollment/waitlist numbers into 
    /resources/201701_proc/{epoch_time}
"""            
if __name__ == "__main__":
    
    proc_history()
    cat_files_to("../resources/data")


# In[ ]:



