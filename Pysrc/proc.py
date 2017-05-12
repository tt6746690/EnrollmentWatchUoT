
# coding: utf-8

# In[11]:

from os import listdir, path
import json

# Holds the timestamp for current json file
CUR_EPOCH_TIME = 0

ARCHIVE_DIR = "../../uoft-timetable-archive/201701/"
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


def proc_one(course, fp):
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
                    sectionNumber: 0501,
                    waitlist: Y,
                    enrollmentCapacity: 18,
                    actualEnrolment: 6,
                    actualWaitlist: 0
                ],
                ...
            ]
    """
    mts = []

    ccode = course["code"][:6]
    sec = course["section"]

    meetings = course["meetings"]
    for k, val in meetings.items():

        # cancelled courses does not have waitlist field
        if val["cancel"] == "Cancelled":
            raise courseCancelledException("{} {} cancelled".format(ccode, k))

        meeting = []
        meeting.append(ccode)
        meeting.append(sec)

        meeting.append(val["meetingId"])
        meeting.append(val["teachingMethod"])
        meeting.append(val["sectionNumber"])
        meeting.append(val["waitlist"])
        meeting.append(val["enrollmentCapacity"])
        meeting.append(val["actualEnrolment"])
        meeting.append(val["actualWaitlist"])

        meeting.append(str(CUR_EPOCH_TIME))
        mts.append(meeting)

        # write current meeting to a line in data file
        fp.write('\t'.join(meeting) + '\n')
    return mts



def proc_entire(jsonp, dest):
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
    mts = []

    with open(jsonp, 'r') as inf:
        with open(dest, 'w+') as outf:

            # process each course in json file
            j = json.load(inf)
            for k, val in j.items():
                try:
                    mts.extend(proc_one(val, outf))
                except KeyError:
                    print("proc {} dict KeyError".format(k))
                except Exception as e:
                    print(e)
                    pass
    return mts

def proc_history():
    """Process every json file archived under
        ARCHIVE_DIR
    """
    for jsonp in listdir(ARCHIVE_DIR):
        if jsonp != "latest.json":
            global CUR_EPOCH_TIME
            CUR_EPOCH_TIME = path.splitext(jsonp)[0]

            proc_entire(path.join(ARCHIVE_DIR, jsonp),
                        path.join(DATA_TABLE_DIR, CUR_EPOCH_TIME))



def cat_files_to(dest):
    """ Concatenate files in
        /resources/201701_proc into
        a file specified by path dest

        str dest
    """
    with open(dest, 'w+') as outf:

        # write header
        outf.write('\t'.join(DATA_TABLE_HEADERS) + '\n')

        for jsonp in listdir(DATA_TABLE_DIR):
            if jsonp != "latest.json":
                with open(path.join(DATA_TABLE_DIR, jsonp)) as inf:
                    outf.write(inf.read())




""" Processes the Time-Table-Archive json from 201701
    by extracting enrollment/waitlist numbers into 
    /resources/201701_proc/{epoch_time}
"""
if __name__ == "__main__":

    proc_history()
    cat_files_to("../resources/data")

    



# In[ ]:



