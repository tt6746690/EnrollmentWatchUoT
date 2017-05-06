# EnrollmentWatchUofT

## Planning
+ Daemon
  + creates a daemon that runs python constantly
+ scrapping
  + constantly check `/api/lastUpdated` to scrape new data
    + compare json files and one downloaded
      + done efficiently, cannot just overwrite
      + update josn files if different
    + populate database with enrollment data  
      + overwrite if changed
      + requires
        + procedure to `read`/`write` data. `write` is assumed to be infrequent
    + efficiency
      + incorporate use of `thread` in downloading about 5000 json files each time
+ storage
  + `course = code-section-session` (`CSC369H1-F-20169`)
  + as json file for each `course`
    + under `/resources/session/subject` (`/resources/20169/`)
    + info related to course, section, instructor, meeting, schedule,
    + assuming general information changes infrequently and only the lastest version is of importance
  + write a simple API for update (i.e. `read`/`write` specific fields)
+ database
  + `enrolment` for each `course`
    + `waitlist`: `bool`
    + `actual_enrolment`: `int`
    + `actual_waitlist`: `int`
    + `timestamp`: `time` as epoch time
+ statistical inference
  + exploratory
    + data that is relevant
      + before Jan 18, 2017 last day to add and change meeting sections in S courses
      + `waitlist = Y`
      + `teachingMethod = LEC`
      + `section = S`
      + `actualWaitlist` is not zero most of the time
    + features  
      + year offer (1-4)
      + enrollmentCapacity
      + department (`CSC`, `STA`)
      + time till last day to drop waitlist
      + `demand = actualEnrolment * actualWaitlist`
  + enrollment/waitlist prediction
    + time series analysis: have to look more into this
    + think about smaller problem that behaves nicer
  + enrollment/waitlist
    + correlation with other features (that may help to infer future..), i.e.  
      + length of lecture
      + instructor
  + use of machine learning in any way?
    + ask others
+ backend: flask
  + keep it simple
+ frontend: react
  + keep it simple




## Objectives
+ refresher on sql/database stuff
+ try out flask/react  
+ statistics problem solving
+ implement part of project with cpp? (think about it)
+ learn how daemon works


```
jupyter nbconvert scraper.py.ipynb --to script
```
