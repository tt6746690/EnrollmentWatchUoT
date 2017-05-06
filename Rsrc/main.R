library(data.table)
library(ggplot2)
library(ResourceSelection)

setwd("~/github/EnrollmentWatchUofT/Rsrc")

# Load data
d_201701_p <- "../resources/data"
d_201701 <- read.table(
    d_201701_p,
    header=TRUE
  )
dt_in <- data.table(d_201701)

# Processing

pattern = "^(CSC)"

dt <- dt_in[section == "S" & teachingMethod == "LEC" &
         waitlist == "Y" & enrollmentCapacity != "9999" &
         grepl(pattern, courseCode) &
         epochTime <= as.POSIXct( as.Date("2017-01-18") ),  # last day to add/change courses
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
        ][
          waitlist_by_name_bar > 0,,       # intersted in active waitlist only
        ][
          , .(code, name, time, enroll, waitlist, capacity, year, waitlist_by_name_bar, days_from_start),
        ]

print(dt)



# multiple regression
# waitlist + capacity + factor(year) +
model <- lm(waitlist ~ capacity + year + days_from_start, data=dt)
summary(model)

plot(dt$waitlist, fitted(model), xlim = c(0, 1), ylim = c(0, 1))




layout(matrix(c(1,2,3,4),2,2)) # optional 4 graphs/page
plot(fit)

# Plotting

ggplot(data = dt, aes(x=time, y=enroll, group=name, colour= year )) + geom_line()
ggplot(data = dt, aes(x=time, y=waitlist, group=name, colour=year)) + geom_line()
