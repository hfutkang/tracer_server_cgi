-- MySQL dump 10.13  Distrib 5.1.73, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: traceinfo
-- ------------------------------------------------------
-- Server version	5.1.73-1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `DeviceInfo`
--

DROP TABLE IF EXISTS `DeviceInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `DeviceInfo` (
  `ID` char(15) NOT NULL,
  `Master` char(11) NOT NULL,
  `Password` char(6) DEFAULT NULL,
  `Activated` char(1) DEFAULT NULL,
  `mnc` int(10) unsigned DEFAULT NULL,
  `mcc` int(10) unsigned DEFAULT NULL,
  `cid` int(10) unsigned DEFAULT NULL,
  `lac` int(10) unsigned DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DeviceInfo`
--

LOCK TABLES `DeviceInfo` WRITE;
/*!40000 ALTER TABLE `DeviceInfo` DISABLE KEYS */;
INSERT INTO `DeviceInfo` VALUES ('19556428477','13424360005','123456','1',0,460,4591,9365,'2015-02-28 18:25:27'),('10874125448','13528738942','123456','1',0,460,3862,9365,'2013-01-22 11:11:11'),('10087','13488888888','hello','1',0,460,4591,9365,'2014-02-22 11:11:11'),('13322222222','13488888888','hello','1',NULL,NULL,NULL,NULL,NULL),('15248852611','13528738942','123456','1',0,460,4591,9365,'2015-01-01 00:02:07');
/*!40000 ALTER TABLE `DeviceInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `EditionInfo`
--

DROP TABLE IF EXISTS `EditionInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `EditionInfo` (
  `version` int(11) NOT NULL,
  `updatetime` date NOT NULL,
  `changes` char(200) DEFAULT NULL,
  `url` char(100) NOT NULL,
  `name` char(20) NOT NULL,
  PRIMARY KEY (`version`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `EditionInfo`
--

LOCK TABLES `EditionInfo` WRITE;
/*!40000 ALTER TABLE `EditionInfo` DISABLE KEYS */;
INSERT INTO `EditionInfo` VALUES (3,'2015-01-13',NULL,'http://192.168.5.253/pub/sct/tracker/Tracker.apk','Tracker_v3.0');
/*!40000 ALTER TABLE `EditionInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `positions`
--

DROP TABLE IF EXISTS `positions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `positions` (
  `clientid` char(20) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  `longitude` char(32) DEFAULT NULL,
  `latituede` char(32) DEFAULT NULL,
  `voltage` float DEFAULT NULL,
  `type` char(1) NOT NULL DEFAULT '1',
  KEY `clientidx` (`clientid`),
  KEY `clnttime` (`clientid`,`time`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `positions`
--

LOCK TABLES `positions` WRITE;
/*!40000 ALTER TABLE `positions` DISABLE KEYS */;
INSERT INTO `positions` VALUES ('13322222222','2014-09-28 12:01:01','(%27-%27)abcdefghi','(%27-%27)abcdefghi',3800,'1'),('13322222222','2014-10-28 12:01:01','123456789','123456789',3800,'1'),('10087','2014-01-22 11:11:11','116.3323169','39.98610916',123,'0'),('19556428477','2015-03-05 16:56:20','116.3323169','39.98610916',3214,'0'),('10874125448','2015-03-06 06:37:56','113562342','022333759',4113,'1'),('10874125448','2015-03-06 06:38:16','113562436','022333705',4109,'1'),('10874125448','2015-03-06 06:38:36','113562555','022333631',4106,'1'),('10874125448','2015-03-06 06:40:37','113563205','022333207',4094,'1'),('10874125448','2015-03-06 06:42:48','113562548','022333579',4107,'1'),('10874125448','2014-01-22 11:11:11','113.93296442','22.56350891',123,'0'),('15248852611','2015-01-01 00:02:07','113.93337492','22.5563345',123,'0'),('10874125448','2014-01-22 11:11:11','113.93296442','22.56350891',6.3313e+06,'0'),('10874125448','2013-01-22 11:11:11','113.93296442','22.56350891',6.3313e+06,'0'),('10874125448','2013-01-22 11:11:11','113.93296442','22.56350891',6.3313e+06,'0'),('10874125448','2013-01-22 11:11:11','113.93296442','22.56350891',888,'0'),('15248852611','2015-03-06 07:22:23','113562579','022333800',3688,'1'),('15248852611','2015-03-06 07:22:43','113562581','022333814',3679,'1'),('15248852611','2015-03-06 07:23:03','113562571','022333855',3673,'1'),('15248852611','2015-03-06 07:23:23','113562580','022333700',3659,'1'),('15248852611','2015-03-06 07:23:43','113562519','022333742',3655,'1'),('15248852611','2015-03-06 07:23:52','113562170','022333992',3659,'1'),('10087','2014-02-22 11:11:11','113.93337492','22.5563345',222,'0');
/*!40000 ALTER TABLE `positions` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-07-16 10:04:26
