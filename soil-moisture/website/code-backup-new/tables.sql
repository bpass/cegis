

CREATE TABLE USERS (
 user_name VARCHAR(20) NOT NULL,
 pass VARCHAR(12) NOT NULL,
 date_joined DATE,
 PRIMARY KEY(user_name)
 )
 
 CREATE TABLE IMAGE_INFO (
 id BIGINT NOT NULL,
 sensor VARCHAR(50),
 lat DOUBLE,
 lon DOUBLE,
 capture_date DATE,
 spect_r DOUBLE,
 spat_r DOUBLE,
 projection VARCHAR(50),
 spheroid VARCHAR(50),
 datum VARCHAR(50),
 PRIMARY KEY(id))

 CREATE TABLE COMMENTS (
   image_id BIGINT,
   comments TEXT,
   FOREIGN KEY(image_id) references IMAGE_INFO(ID)
)
