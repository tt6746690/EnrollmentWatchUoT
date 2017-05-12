STORAGE_DIR = "../resources/"

# scrap
TIMETABLE_API_URL = "https://timetable.iit.artsci.utoronto.ca/api/"
CUR_SESSION = "20169"
JSON_STORAGE_PATH = STORAGE_DIR + CUR_SESSION


# proc 
ARCHIVE_DIR = "../../uoft-timetable-archive/201701/"
DATA_TABLE_DIR = STORAGE_DIR + "201701_proc/"
DATA_201701 = STORAGE_DIR + "data"

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


# analysis 
WAITLIST_CLOSE_DATE = "2017-01-15"
ENROLLMENT_CLOSE_DATE = "2017-01-18"

# true: enrollment; false: waitlist
INFO_FLAG = False       
CUTOFF_DATE = ENROLLMENT_CLOSE_DATE if(INFO_FLAG) else WAITLIST_CLOSE_DATE


# first year.. 
COURSE_CODES = ["CSC[1,2]", "STA247", "STA248", "MAT223"]
NOT_COURSE_CODES = ["CSC108"]

# second year..
COURSE_CODES = ["CSC2", "MAT237", "STA257", "STA261"]
NOT_COURSE_CODES = ["HAS_TO_BE_HERE_STRING"]

# third year..
COURSE_CODES = ["CSC3"]
NOT_COURSE_CODES = ["HAS_TO_BE_HERE_STRING"]
