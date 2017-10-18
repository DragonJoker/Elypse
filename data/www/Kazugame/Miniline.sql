-- phpMyAdmin SQL Dump
-- version OVH
-- http://www.phpmyadmin.net
--
-- Host: mysql5-10
-- Generation Time: Apr 09, 2009 at 12:16 PM
-- Server version: 5.0.68
-- PHP Version: 4.4.9

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `Miniline`
--

--
-- Table structure for table `chat_friends`
--

CREATE TABLE IF NOT EXISTS `chat_friends` (
  `ref_user` int NOT NULL default '0',
  `ref_friend` int NOT NULL default '0',
  PRIMARY KEY  (`ref_user`,`ref_friend`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `chat_friends`
--


-- --------------------------------------------------------

--
-- Table structure for table `chat_ignored`
--

CREATE TABLE IF NOT EXISTS `chat_ignored` (
  `ref_user` int(8) NOT NULL default '0',
  `ref_ignored` int(8) NOT NULL default '0',
  PRIMARY KEY  (`ref_user`,`ref_ignored`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `chat_ignored`
--


-- --------------------------------------------------------

--
-- Table structure for table `chat_room`
--

CREATE TABLE IF NOT EXISTS `chat_room` (
  `id_room` int(8) NOT NULL auto_increment,
  `name` varchar(30) NOT NULL default '',
  `scene_name` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`id_room`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Dumping data for table `chat_room`
--

INSERT INTO `chat_room` (`id_room`, `name`, `scene_name`) VALUES
(1, 'Eden', 'Eden'),
(2, 'Enfer', 'Enfer'),
(3, 'Paradiz', 'Paradiz');

-- --------------------------------------------------------

--
-- Table structure for table `games`
--

CREATE TABLE IF NOT EXISTS `games` (
  `id_game` int(8) NOT NULL auto_increment,
  `name` varchar(32) NOT NULL default '',
  `pict` varchar(128) NOT NULL default '',
  `pict_mini` varchar(128) NOT NULL default '',
  `pict_titre` varchar(128) NOT NULL default '',
  `muse_file` varchar(128) NOT NULL default '',
  `height` int(8) NOT NULL default '600',
  `width` int(8) NOT NULL default '800',
  `price_play` int(8) NOT NULL default '0',
  `price_abonnement` int(8) NOT NULL default '0',
  `limit_free` int(8) NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '1',
  `max_players` int(8) NOT NULL default '1',
  `current_players` int(8) NOT NULL default '0',
  PRIMARY KEY  (`id_game`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=10 ;

--
-- Dumping data for table `games`
--

INSERT INTO `games` (`id_game`, `name`, `pict`, `pict_mini`, `pict_titre`, `muse_file`, `height`, `width`, `price_play`, `price_abonnement`, `limit_free`, `active`) VALUES
(1, 'Golden Pyramid', 'images/GoldenPyramid.jpg', 'images/GoldenPyramid.jpg', 'images/GoldenPyramid.gif', 'GoldenPyramid.muse', 480, 640, 15, 50, 3, 1),
(2, 'Tombeau Rider', 'images/TombeauRider.jpg', 'images/TombeauRider.jpg', 'images/TombeauRider.gif', 'muse/TombeauRider.muse', 480, 640, 15, 450, 3, 1),
(3, 'Flipper', 'images/Flipper.jpg', 'images/Flipper.jpg', 'images/Flipper.gif', 'muse/Flipper.muse', 480, 640, 15, 50, 3, 1),
(4, 'Surf', 'images/Surf.jpg', 'images/Surf.jpg', 'images/Surf.gif', 'muse/Surf.muse', 480, 640, 15, 50, 3, 1),
(5, 'Wall Destruction', 'images/WallDestruction.jpg', 'images/WallDestruction.jpg', 'images/WallDestruction.gif', 'muse/WallDestruction.muse', 480, 640, 15, 50, 3, 1),
(6, 'Bassine-Jump', 'images/BassineJump.jpg', 'images/BassineJump.jpg', 'images/BassineJump.gif', 'muse/BassineJump.muse', 480, 640, 15, 50, 3, 1),
(7, 'Mocking Ghost', 'images/MockingGhost.jpg', 'images/MockingGhost.jpg', 'images/MockingGhost.gif', 'muse/MockingGhost.muse', 480, 640, 15, 50, 3, 1),
(8, 'Gratz Paire', 'images/GratzPaire.jpg', 'images/GratzPaire.jpg', 'images/GratzPaire.gif', 'muse/GratzPaire.muse', 480, 640, 15, 0, 3, 1),
(9, '421', 'images/421.jpg', 'images/421.jpg', 'images/421.jpg', 'muse/421.muse', 480, 640, 15, 0, 3, 0);

-- --------------------------------------------------------

--
-- Table structure for table `game_desc`
--

CREATE TABLE IF NOT EXISTS `game_desc` (
  `ref_game` int(8) NOT NULL default '0',
  `code_lang` char(2) NOT NULL default '',
  `desc` text NOT NULL,
  PRIMARY KEY  (`ref_game`,`code_lang`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `game_desc`
--

INSERT INTO `game_desc` (`ref_game`, `code_lang`, `desc`) VALUES
(1, 'FR', '<div align=justify>\r\nLe bandit manchot ! le jeu le plus simple qui soit. Faites exploser la banque!<br>\r\n</div>\r\n<BR>'),
(1, 'EN', 'play and win'),
(2, 'FR', 'Une pyramide infestée de momies ... fuyez!<br>\r\n<FONT size=\\"-3\\">*Version du jeu non comptabilisée dans les scores pour les concours en cours de validités</FONT>\r\n\r\n'),
(2, 'EN', 'Golden Pyramide : '),
(3, 'FR', 'A vous de monter le plus haut possible !<br>\r\n<FONT size=\\"-3\\">*Version du jeu non comptabilisé dans les scores pour les concours en cours de validités</FONT> \r\n'),
(3, 'EN', 'Flipper : '),
(4, 'FR', '<div align=justify>\r\nUn jeu de typing très sympathique mais un peut ... stressant ! A essayer <br>\r\n</div>'),
(4, 'EN', 'Surf'),
(5, 'FR', '<div align=justify>\r\nWall destruction !! Je vous met au defis de casser le mur entier ! <br>\r\n</div>'),
(5, 'EN', 'Wall destruction : \r\n\r\ngggg'),
(6, 'FR', '<div align=justify>\r\nUn homme saute d\\''un gratte-ciel. Votre défi : atterrir dans un verre d\\''eau ... vous avez bien lu !<br>\r\n</div>\r\n<br>'),
(6, 'EN', 'Bassine Jump'),
(7, 'FR', '<div align=justify>\r\nSimple et efficace : trouver la pièce d\\''or. A essayer ! <br>\r\n</div>'),
(7, 'EN', 'Mocking Ghost'),
(8, 'FR', '<div align=justify>\r\nUn jeux de mémoire. Réalisez des paires de cartes. <br>\r\n</div>'),
(8, 'EN', 'gratz-pair'),
(9, 'FR', '<div align=justify>\r\nVous connaissez ? Trois coup possible en lancant 3 dès pour avoir le \\"quatre\\" le \\"deux\\" et le \\"un\\".<br><br>\r\n</div> \r\n<br>'),
(9, 'EN', '');

-- --------------------------------------------------------

--
-- Table structure for table `language`
--

CREATE TABLE IF NOT EXISTS `language` (
  `id_lang` int(8) NOT NULL auto_increment,
  `code` char(2) NOT NULL default '',
  `name` varchar(30) NOT NULL default '',
  `img` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`id_lang`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3 ;

--
-- Dumping data for table `language`
--

INSERT INTO `language` (`id_lang`, `code`, `name`, `img`) VALUES
(1, 'FR', 'Français', 'Language/FR/francais.jpg'),
(2, 'EN', 'English', 'Language/EN/english.jpg');

-- --------------------------------------------------------

--
-- Table structure for table `logs_highscore`
--

CREATE TABLE IF NOT EXISTS `logs_highscore` (
  `id_log` int(8) NOT NULL auto_increment,
  `ref_user` int(8) NOT NULL default '0',
  `ref_game` int(8) NOT NULL default '0',
  `time` datetime NOT NULL default '0000-00-00 00:00:00',
  `score` int(8) NOT NULL default '0',
  PRIMARY KEY  (`id_log`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `logs_highscore`
--

-- --------------------------------------------------------

--
-- Table structure for table `tbl_user`
--

CREATE TABLE IF NOT EXISTS `tbl_user` (
  `id_user` int(8) NOT NULL auto_increment,
  `sexe` varchar(6) NOT NULL default 'homme',
  `login` varchar(32) NOT NULL default '',
  `pwd` varchar(40) NOT NULL default '',
  `admin` tinyint(1) NOT NULL default '0',
  `mail` varchar(40) NOT NULL default '',
  `avatar` text NOT NULL,
  `rue` text NOT NULL,
  `ville` varchar(128) NOT NULL default '',
  `codepostal` int(6) NOT NULL default '0',
  `credits` int(8) NOT NULL default '0',
  `banned` tinyint(1) NOT NULL default '0',
  `pays` varchar(30) NOT NULL default '',
  `dresses` varchar(16) NOT NULL default '',
  `dob` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`id_user`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `tbl_user`
--

INSERT INTO `tbl_user` (`id_user`, `sexe`, `login`, `pwd`, `admin`, `mail`, `avatar`, `rue`, `ville`, `codepostal`, `credits`, `banned`, `pays`, `dresses`, `dob`) VALUES
(1, 'homme', 'dragonjoker', '8b5ea33b8743b23ee23817b59f4aaa06', 1, 'dragonjoker', 'upload/ad7bc863acc50ad3b747c51c2f85b4311238171223.jpg', '5 rue Maurice Thorez', 'Nanterre', 92000, 999999900, 0, 'France', '@@EJ@G@@@@@', '0000-00-00');

-- --------------------------------------------------------

--
-- Table structure for table `mini_slots`
--

CREATE TABLE IF NOT EXISTS `mini_slots` (
  `id_slot` int(8) NOT NULL auto_increment,
  `sex` int(8) NOT NULL,
  PRIMARY KEY  (`id_slot`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_slots`
--
INSERT INTO `mini_slots` (`id_slot`, `sex`) VALUES
(1, 1);



-- --------------------------------------------------------

--
-- Table structure for table `mini_dresses`
--

CREATE TABLE IF NOT EXISTS `mini_dresses` (
  `id_dress` int(8) NOT NULL auto_increment,
  `sex` int(8) NOT NULL,
  PRIMARY KEY (`id_dress`,`sex`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_dresses`
--
INSERT INTO `mini_dresses` (`id_dress`, `sex`) VALUES
(1, 1);



-- --------------------------------------------------------

--
-- Table structure for table `mini_dress_slots`
--

CREATE TABLE IF NOT EXISTS `mini_dress_slots` (
  `ref_dress` int(8) NOT NULL,
  `ref_slot` int(8) NOT NULL,
  PRIMARY KEY  (`ref_dress`, `ref_slot`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_dress_slots`
--
INSERT INTO `mini_dress_slots` (`ref_dress`, `ref_slot`) VALUES
(1, 1);


-- --------------------------------------------------------

--
-- Table structure for table `mini_player_dress`
--

CREATE TABLE IF NOT EXISTS `mini_player_dress` (
  `id_player_dress` int(8) NOT NULL auto_increment,
  `ref_slot` int(8) NOT NULL,
  `ref_dress` int(8) NOT NULL,
  `ref_player` int(8) NOT NULL,
  PRIMARY KEY  (`id_player_dress`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_player_dress`
--
INSERT INTO `mini_player_dress` (`id_player_dress`, `ref_slot`, `ref_dress`, `ref_player`) VALUES
(1, 1, 1, 1);



-- --------------------------------------------------------

--
-- Table structure for table `mini_player_dress_mat`
--

CREATE TABLE IF NOT EXISTS `mini_player_dress_mat` (
  `ref_player_dress` int(8) NOT NULL,
  `ref_dress_mat` int(8) NOT NULL,
  `submat` int(8) NOT NULL
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_player_dress_mat`
--
INSERT INTO `mini_player_dress_mat` (`ref_player_dress`, `ref_dress_mat`, `submat`) VALUES
(1, 1, 1);



-- --------------------------------------------------------

--
-- Table structure for table `mini_dress_mats`
--

CREATE TABLE IF NOT EXISTS `mini_dress_mats` (
  `ref_dress` int(8) NOT NULL,
  `submat_index` int(8) NOT NULL
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_dress_mats`
--
INSERT INTO `mini_dress_mats` (`ref_dress`, `submat_index`) VALUES
(1, 0);


-- --------------------------------------------------------

--
-- Table structure for table `mini_paint_slots`
--

CREATE TABLE IF NOT EXISTS `mini_paint_slots` (
  `id_slot` int(8) NOT NULL auto_increment,
  `sex` int(8) NOT NULL,
  PRIMARY KEY  (`id_slot`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_paint_slots`
--
INSERT INTO `mini_paint_slots` (`id_slot`, `sex`) VALUES
(1, 1);



-- --------------------------------------------------------

--
-- Table structure for table `mini_tattoos`
--

CREATE TABLE IF NOT EXISTS `mini_tattoos` (
  `id_tatoo` int(8) NOT NULL auto_increment,
  `sex` int(8) NOT NULL,
  PRIMARY KEY  (`id_tatoo`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_dresses`
--
INSERT INTO `mini_tattoos` (`id_tatoo`, `sex`) VALUES
(1, 1);


-- --------------------------------------------------------

--
-- Table structure for table `mini_tattoo_slots`
--

CREATE TABLE IF NOT EXISTS `mini_tattoo_slots` (
  `ref_tattoo` int(8) NOT NULL,
  `ref_slot` int(8) NOT NULL,
  PRIMARY KEY  (`ref_tattoo`, `ref_slot`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_dress_slots`
--
INSERT INTO `mini_tattoo_slots` (`ref_tattoo`, `ref_slot`) VALUES
(1, 1);


-- --------------------------------------------------------

--
-- Table structure for table `mini_player_tattoo`
--

CREATE TABLE IF NOT EXISTS `mini_player_tattoo` (
  `id_player_tattoo` int(8) NOT NULL auto_increment,
  `ref_slot` int(8) NOT NULL,
  `ref_tattoo` int(8) NOT NULL,
  `ref_player` int(8) NOT NULL,
  PRIMARY KEY  (`id_player_tattoo`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Dumping data for table `mini_player_tattoo`
--
INSERT INTO `mini_player_tattoo` (`id_player_tattoo`, `ref_slot`, `ref_tattoo`, `ref_player`) VALUES
(1, 1, 1, 1);


