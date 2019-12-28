-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema smart_bag_db
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema smart_bag_db
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `smart_bag_db` DEFAULT CHARACTER SET utf8 ;
USE `smart_bag_db` ;

-- -----------------------------------------------------
-- Table `smart_bag_db`.`accounts`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `smart_bag_db`.`accounts` (
  `acc_id` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(45) NULL,
  `fname` VARCHAR(45) NULL,
  `lname` VARCHAR(45) NULL,
  `email` VARCHAR(45) NULL,
  `profile_img` VARCHAR(255) NULL,
  `phone_num` VARCHAR(45) NULL,
  `password` VARCHAR(45) NULL,
  PRIMARY KEY (`acc_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `smart_bag_db`.`registered_prints`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `smart_bag_db`.`registered_prints` (
  `print_id` INT NOT NULL,
  `fname` VARCHAR(45) NULL,
  `lname` VARCHAR(45) NULL,
  `phone_num` VARCHAR(45) NULL,
  `accounts_acc_id` INT NOT NULL,
  PRIMARY KEY (`print_id`),
    FOREIGN KEY (`accounts_acc_id`)
    REFERENCES `smart_bag_db`.`accounts` (`acc_id`)
    )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `smart_bag_db`.`notifs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `smart_bag_db`.`notifs` (
  `notif_id` INT NOT NULL AUTO_INCREMENT,
  `status` VARCHAR(45) NULL,
  `user` VARCHAR(45) NULL,
  `time` DATETIME DEFAULT CURRENT_TIMESTAMP,
  `accounts_acc_id` INT NOT NULL,
  `registered_prints_print_id` INT NULL,
  PRIMARY KEY (`notif_id`),
    FOREIGN KEY (`accounts_acc_id`)
    REFERENCES `smart_bag_db`.`accounts` (`acc_id`),
    FOREIGN KEY (`registered_prints_print_id`)
    REFERENCES `smart_bag_db`.`registered_prints` (`print_id`)
    )
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
