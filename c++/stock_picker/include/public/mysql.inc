	const char *_db_name = "stocks";
	const char *_tables[4] = {
		R"(CREATE TABLE IF NOT EXISTS `stocks_info`(
            `stock_code` INT UNSIGNED,
            `stock_name` VARCHAR(50) NOT NULL,
            `pe` INT NOT NULL,
            `market_value` BIGINT NOT NULL,
			`traded_market_value` BIGINT NOT NULL, 
            PRIMARY KEY ( `stock_code` )
        )ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `sectors`(
            `id` INT UNSIGNED AUTO_INCREMENT,
            `sector_name` VARCHAR(100) NOT NULL,
            PRIMARY KEY ( `id` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `stock_sector_related`(
            `id` INT UNSIGNED AUTO_INCREMENT,
            `stock_code` INT UNSIGNED NOT NULL,
            `sector_id` INT UNSIGNED NOT NULL,
            PRIMARY KEY ( `id` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)", 
		R"(CREATE TABLE IF NOT EXISTS `stocks_history`(
            `stock_code` INT UNSIGNED,
            `created_date` CHAR(12),
            `rank` INT UNSIGNED NOT NULL,
            `price_start` INT UNSIGNED NOT NULL,
            `price_newst` INT UNSIGNED NOT NULL,
            `price_higest` INT UNSIGNED NOT NULL,
            `price_lowest` INT UNSIGNED NOT NULL,
            `turnover_rate` INT NOT NULL,
            `turnover_sum` BIGINT UNSIGNED NOT NULL,
            `amplitude` INT UNSIGNED NOT NULL,
            `change_percent` INT NOT NULL,
            PRIMARY KEY ( `stock_code`, `created_date` )
		)ENGINE=InnoDB DEFAULT CHARSET=utf8;
		)" 
	};
