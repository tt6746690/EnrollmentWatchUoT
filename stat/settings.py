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

FIG_SIZE = (18, 18)

BATCH_LINE_GRAPH = [
    {
        "image": "./graphs/csc_y1.png",
        "courses": ["CSC1", "MAT135", "MAT136", "MAT223"],
        "not_courses": ["CSC108"]
    },
    {
        "image": "./graphs/csc_y2.png",
        "courses": ["CSC2", "STA247", "STA248", "MAT237"],
        "not_courses": ["(ignoreme)"]
    },
    {
        "image": "./graphs/csc_y3.png",
        "courses": ["CSC3"],
        "not_courses": ["(ignoreme)"]
    },
    {
        "image": "./graphs/csc_y3_no321.png",
        "courses": ["CSC3"],
        "not_courses": ["CSC321", "CSC309"]
    },
    
]
