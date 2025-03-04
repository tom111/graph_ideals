#pragma once

#include "stdafx.h"
#include "sqlite3.h"

#include "Graph.h"
#include "BettiTable.h"


class DatabaseInterface
{
private:
	sqlite3 * database;

	unsigned number_columns;
	unsigned number_rows;

	std::vector<std::string> view_columns;
	std::vector<std::vector<std::string>> view_contents;

	std::vector<unsigned> column_widths;

public:
	DatabaseInterface() {
	    database = nullptr;
	}

	DatabaseInterface(const char * database_file_name) : number_columns(0), number_rows(0), view_columns{}, view_contents{}, column_widths{} {
		if (sqlite3_open(database_file_name, &database))
		{
			std::cout << "Unable to open database: " << sqlite3_errmsg(database) << std::endl;
			database = 0;
		}
	}

	DatabaseInterface& operator=(DatabaseInterface && dbi) {
	    if (this != &dbi) {
		if (database) {
		    sqlite3_close_v2(database);
		}
		database = dbi.database;
		dbi.database = nullptr;
		reset_view();
	    }
	    return *this;
	}

	~DatabaseInterface() {
	    if (database) {
		sqlite3_close_v2(database);
	    }
	}

	void reset_view();

	void show_view(int limit = 100);
	void save_view(std::ofstream * file);

	void show_view_rich(int limit = 25);
	void save_view_rich(std::ofstream * file);

	void save_view_visualisation(std::ofstream * file);
	void save_view_g6(std::ofstream * file);

	void generate_m2_scripts(std::string * idealname, unsigned * (Graph::*gen_ordering)(), unsigned batch_size, const char * query_condition, const char * filename, const char * ordering_name, unsigned index);

	void show_scripts();

	bool execute_SQL_query(std::string * query);
	bool execute_SQL_statement(std::string * statement);

	bool create_scripts_table();
	bool create_graphs_table();

	void import_graphs(std::ifstream * file, bool (Graph::*Read_next_format)(std::ifstream * file));

	bool update_type(bool (Graph::*graph_test)(), const char * type, const char * query_condition);
	bool update_values(std::vector<unsigned>(Graph::*graph_values)(), std::vector<const char *> * columns, const char * query_condition);

	unsigned find_script_data(unsigned scriptID, std::string * ideal, std::string * query_condition, std::string * datetime);
	bool insert_betti_data(std::string * ideal, std::string * query_condition, std::string * datetime, unsigned index);
	bool insert_hpoldeg_data(std::string * ideal, std::string * query_condition, std::string * datetime, unsigned index);
};
