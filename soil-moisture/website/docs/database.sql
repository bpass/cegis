
CREATE TABLE image_info (
ID		NUMBER(6) NOT NULL, 
SATELLITE	VARCHAR2(50),
SENSOR		VARCHAR2(50),
LAT		NUMBER(10,4),
LON		NUMBER(10,4),
LS_PATH		NUMBER(6),
LS_ROW		NUMBER(6),
CAPTURE_DATE	DATE,
SPAT_R		NUMBER(10,4),
PROJECTION	VARCHAR2(100),
DATUM		VARCHAR2(100),
SPHEROID	VARCHAR2(100),
COMMENTS	VARCHAR2(2000)
)

CREATE TABLE project_news (
ID                           NUMBER(6) NOT NULL,
POST_DATE                     DATE,
ARTICLE                       VARCHAR2(4000),
SUBJECT                       VARCHAR2(500)
) 

CREATE SEQUENCE news_seq 
MINVALUE 1
MAXVALUE 999999
START WITH 1 
INCREMENT by 1

CREATE SEQUENCE id_seq
MINVALUE 1
MAXVALUE 999999
START WITH 1 
INCREMENT by 1

//Don't forget to grant soilmoisture update, select, and insert permissions on tables
//and update permissions on id_seq and news_seq