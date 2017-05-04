library(data.table)
library(ggplot2)

setwd("~/github/waitlist/Rsrc")

# Load data
d_201701_p <- "../resources/data"
d_201701 <- read.table(
    d_201701_p,
    header=TRUE
  )
dt <- data.table(d_201701)

# Processing

dt <- dt[section == "S" & teachingMethod == "LEC" &
         waitlist == "Y" & enrollmentCapacity != "9999" &
         grepl("^(MAT|STA)", courseCode) &
         epochTime <= as.POSIXct( as.Date("2017-01-18") ),  # last day to add/change courses
        ][
          , ':=' (
              code = substr(courseCode, 0, 3),
              name = paste(courseCode, sectionNumber, sep='_'),
              enroll = actualEnrolment/enrollmentCapacity,
              waitlist = actualWaitlist/enrollmentCapacity,
              time = as.Date(as.POSIXct(epochTime, origin="1970-01-01"))
            ),
        ][
          , .(code, name, enroll, waitlist, time),
        ]

print(dt)

# Plotting

ggplot(data = dt, aes(x=time, y=enroll, group=name, colour=code)) + geom_line()
ggplot(data = dt, aes(x=time, y=waitlist, group=name, colour=code)) + geom_line()
