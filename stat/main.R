library(data.table)
library(ggplot2)
library(ResourceSelection)

setwd("~/github/EnrollmentWatchUofT/Rsrc")
# source("main.R", print.eval=TRUE)

# Load data
d_201701_p <- "../resources/data"
d_201701 <- read.table(
    d_201701_p,
    header=TRUE
  )
dt_in <- data.table(d_201701)

# Processing

# if interested in enrollment/waitlist info
INFO_FLAG <- FALSE

COURSE_CODE_PATTERN <- "^(CSC)"

WAITLIST_CLOSE_DATE <- "2017-01-15"
ENROLLMENT_CLOSE_DATE <- "2017-01-18"
CUTOFF_DATE <- if(INFO_FLAG == TRUE) ENROLLMENT_CLOSE_DATE else WAITLIST_CLOSE_DATE


dt <- dt_in[section == "S" & teachingMethod == "LEC" &
         waitlist == "Y" & enrollmentCapacity != "9999" &
         grepl(COURSE_CODE_PATTERN, courseCode) &
         epochTime <= as.POSIXct( as.Date(CUTOFF_DATE) ),  # last day to add/change courses
        ][
          , ':=' (
              code = substr(courseCode, 0, 3),
              name = paste(courseCode, sectionNumber, sep='_'),
              time = as.Date(as.POSIXct(epochTime, origin="1970-01-01")),

              # independent variables as estimators
              enroll = actualEnrolment/enrollmentCapacity,       # continuous
              waitlist = actualWaitlist/enrollmentCapacity,      # continuous
              capacity = enrollmentCapacity,                     # continuous
              year = substr(courseCode, 4, 4)                    # ordinal
          ),
        ][
          , ':=' (
              days_from_start = difftime(                        # ordinal
                time,
                as.Date("2016-12-23"),  # start date
                units = c("days")),
              waitlist_by_name_bar = mean(waitlist)
            ),
            by=name
        ]
        [
          waitlist_by_name_bar > 0.05,,       # intersted in active waitlist only
                                              # i.e. waitlist occupied approx 5/100 
        ][
          , .(code, name, time, enroll, waitlist, capacity, year, waitlist_by_name_bar, days_from_start),
        ]

# sink("outputFile")
write.table(dt, "myFile")



# multiple regression
# waitlist + capacity + factor(year) +
model <- lm(waitlist ~ days_from_start, data=dt)
summary(model)


# Plotting

ggplot(data = dt, aes(x=time, y=enroll, group=name, colour=name)) + geom_line()
ggplot(data = dt, aes(x=time, y=waitlist, group=name, colour=name)) + geom_line()
