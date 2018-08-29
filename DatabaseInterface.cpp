#include "DatabaseInterface.h"


#define PI 3.14159265
#define max_column_width 66

//########## helper functions ##########
/**
 * converts str to unsigned
**/
inline unsigned str_to_unsigned(std::string * str) {
	unsigned number = 0;
	for (unsigned i = 0; i < str->length(); i++)
	{
		number *= 10;
		number += str->at(i) - '0';
	}
	return number;
}


/**
 * returns the appropriate coordinates for vertex to print the graph of given order on a circle
**/
std::pair<int, int> get_coordinate(unsigned vertex, unsigned order) {
	int first = (int)100 * cos(2 * (vertex - 1) * PI / order);
	int second = (int)100 * sin(2 * (vertex - 1) * PI / order);

	return std::pair<int, int>(first, second);
}


//########## public member functions ##########
/**
 * resets all member variables excepts the database itself
**/
void DatabaseInterface::reset_view() {
	number_columns = 0;
	number_rows = 0;
	view_columns = {};
	view_contents = {};
	column_widths = {};
}


/**
 * outputs the current view to the terminal
**/
void DatabaseInterface::show_view(int limit) {
	if (number_columns != 0)
	{
		for (unsigned j = 0; j < number_columns - 1; j++)
			std::cout << view_columns[j] << std::string(column_widths[j] - view_columns[j].length(), ' ');
		std::cout << view_columns[number_columns - 1] << "\n";

		for (int i = 0; i < (int)number_rows; i++)
		{
			if (limit != -1
				&& i >= limit)
			{
				std::cout << "\n ... [View exceeds " << limit << " rows. For the complete list please save as text file or show view with higher limit.]\n";
				break;
			}

			for (unsigned j = 0; j < number_columns - 1; j++)
				std::cout << view_contents[i][j] << std::string(column_widths[j] - view_contents[i][j].length(), ' ');
			std::cout << view_contents[i][number_columns - 1] << "\n";
		}
	}

	std::cout << std::endl;
}


/**
* outputs the current view to given file
**/
void DatabaseInterface::save_view(std::ofstream * file) {
	if (number_columns != 0)
	{
		for (unsigned j = 0; j < number_columns - 1; j++)
			*file << view_columns[j] << std::string(column_widths[j] - view_columns[j].length(), ' ');
		*file << view_columns[number_columns - 1] << "\n";

		for (unsigned i = 0; i < number_rows; i++)
		{
			for (unsigned j = 0; j < number_columns - 1; j++)
				*file << view_contents[i][j] << std::string(column_widths[j] - view_contents[i][j].length(), ' ');
			*file << view_contents[i][number_columns - 1] << "\n";
		}
	}
}


/**
* outputs the current view to the terminal in rich format
**/
void DatabaseInterface::show_view_rich(int limit) {
	if (number_columns != 0)
	{
		std::vector<unsigned> matrix_indices;
		std::vector<std::string> table_names;
		std::string columns = "";

		unsigned last_whitespace = 0;
		unsigned overall_width = 0;

		for (unsigned j = 0; j < number_columns; j++)
		{
			std::string column = view_columns[j];

			if (column.find("Bettis") != std::string::npos)
			{
				matrix_indices.push_back(j);
				table_names.push_back(column);
			}
			else
			{
				columns += std::string(last_whitespace, ' ') + column;
				last_whitespace = column_widths[j] - view_columns[j].length();
				overall_width += column_widths[j];
			}
		}

		unsigned index;

		for (int i = 0; i < (int)number_rows; i++)
		{
			if (limit != -1
				&& i >= limit)
			{
				std::cout << "\n ... [View exceeds " << limit << " rows. For the complete list please save as text file or show view with higher limit.]\n";
				break;
			}

			std::cout << std::string(overall_width, '-') << "\n" << columns << "\n";

			index = 0;
			std::vector<BettiTable> tables;
			std::string row = "";

			last_whitespace = 0;

			for (unsigned j = 0; j < number_columns; j++)
			{
				if (index < matrix_indices.size() && j == matrix_indices[index])
				{
					BettiTable b;
					b.read_from_line(&view_contents[i][j]);
					tables.push_back(b);
					index++;
				}
				else
				{
					row += std::string(last_whitespace, ' ') + view_contents[i][j];
					last_whitespace = column_widths[j] - view_contents[i][j].length();
				}
			}
			std::cout << row << "\n";

			if (!tables.empty())
				std::cout << "\n";

			std::cout << BettiTable::convert_tables_to_string(&tables, &table_names);
		}
	}

	std::cout << std::endl;
}


/**
* outputs the current view to given file in rich format
**/
void DatabaseInterface::save_view_rich(std::ofstream * file) {
	if (number_columns != 0)
	{
		std::vector<unsigned> matrix_indices;
		std::vector<std::string> table_names;
		std::string columns = "";

		unsigned last_whitespace = 0;
		unsigned overall_width = 0;

		for (unsigned j = 0; j < number_columns; j++)
		{
			std::string column = view_columns[j];

			if (column.find("Bettis") != std::string::npos)
			{
				matrix_indices.push_back(j);
				table_names.push_back(column);
			}
			else
			{
				columns += std::string(last_whitespace, ' ') + column;
				last_whitespace = column_widths[j] - view_columns[j].length();
				overall_width += column_widths[j];
			}
		}

		unsigned index;

		for (int i = 0; i < (int)number_rows; i++)
		{
			*file << std::string(overall_width, '-') << "\n" << columns << "\n";

			index = 0;
			std::vector<BettiTable> tables;
			std::string row = "";

			last_whitespace = 0;

			for (unsigned j = 0; j < number_columns; j++)
			{
				if (index < matrix_indices.size() && j == matrix_indices[index])
				{
					BettiTable b;
					b.read_from_line(&view_contents[i][j]);
					tables.push_back(b);
					index++;
				}
				else
				{
					row += std::string(last_whitespace, ' ') + view_contents[i][j];
					last_whitespace = column_widths[j] - view_contents[i][j].length();
				}
			}
			*file << row << "\n";

			if (!tables.empty())
				*file << "\n";

			*file << BettiTable::convert_tables_to_string(&tables, &table_names);
		}
	}
}


/**
 * outputs all graphs of the current view to given file in LaTeX-TikZ format (also outputs necessary LaTeX-package includes)
**/
void DatabaseInterface::save_view_visualisation(std::ofstream * file) {
	int graphID_index = -1;
	int graphOrder_index = -1;
	int edges_index = -1;

	for (unsigned i = 0; i < number_columns; i++)
	{
		if (view_columns[i] == "graphID")
		{
			if (graphID_index == -1)
				graphID_index = i;
			else
			{
				FAIL("Saving view", "Visualisation does not support multiple graphs per row.");
				return;
			}
		}
		else if (view_columns[i] == "graphOrder")
		{
			if (graphOrder_index == -1)
				graphOrder_index = i;
			else
			{
				FAIL("Saving view", "Visualisation does not support multiple graphs per row.");
				return;
			}
		}
		else if (view_columns[i] == "edges")
		{
			if (edges_index == -1)
				edges_index = i;
			else
			{
				FAIL("Saving view", "Visualisation does not support multiple graphs per row.");
				return;
			}
		}
	}

	if (graphOrder_index == -1
		|| edges_index == -1)
	{
		FAIL("Saving view", "View does not include both graphOrder and edges. These are necessary to visualize the graphs.");
		return;
	}

	*file << "LaTeX code snippets for visualisation of graphs with TikZ\n" \
		"\n%%########## includes ##########\n\n" \
		"\\usepackage{tikz}\n\\usetikzlibrary{ decorations.pathreplacing }\n\\usetikzlibrary{ shapes.misc }\n\\usetikzlibrary{ calc }\n" \
		"\n%%########## document code ##########\n\n";

	for (unsigned i = 0; i < number_rows; i++)
	{
		std::string info = "";
		if (graphID_index != -1)
			info += view_columns[graphID_index] + ": " + view_contents[i][graphID_index] + ", ";
		else
			info += "Graph " + std::to_string(i + 1) + ", ";
		info += view_columns[graphOrder_index] + ": " + view_contents[i][graphOrder_index];
		info += "\n\n\\begin{center}\n\\begin{tikzpicture}[\nscale=0.04,mynode/.style={draw,fill=white,circle,outer sep=4pt,inner sep=2pt},myedge/.style={line width=1.5,black}\n]\n";
		*file << info;

		unsigned graphOrder;
		std::stringstream order_stream(view_contents[i][graphOrder_index]);
		order_stream >> graphOrder;

		Graph g(graphOrder, &view_contents[i][edges_index]);

		std::string coordinates = "";
		std::string edges = "";
		std::string vertices = "";

		for (unsigned v = 1; v <= graphOrder; v++)
		{
			std::pair<int, int> coordinate = get_coordinate(v, graphOrder);
			coordinates += "\\coordinate(p" + std::to_string(v) + ") at(" + std::to_string(coordinate.first) + "," + std::to_string(coordinate.second) + ");\n";

			for (unsigned w = v + 1; w <= graphOrder; w++)
			{
				if (g.adjacent(v, w))
					edges += "\\draw[myedge] (p" + std::to_string(v) + ") -- (p" + std::to_string(w) + ");\n";
			}

			vertices += "\\node[mynode] at (p" + std::to_string(v) + ") {};\n";
		}
		coordinates += "\n";
		edges += "\n";

		info = "\\end{tikzpicture}\n\\end{center}\n\n";
		if (i % 2 == 1) {
			info += "\\clearpage\n\n";
		}

		*file << coordinates << edges << vertices << info;
	}
}


/**
* outputs all graphs in the current view to given file in g6 format
**/
void DatabaseInterface::save_view_g6(std::ofstream * file) {
	int graphOrder_index = -1;
	int edges_index = -1;

	for (unsigned j = 0; j < number_columns; j++)
	{
		if (view_columns[j] == "graphOrder")
			graphOrder_index = j;
		else if (view_columns[j] == "edges")
			edges_index = j;
	}

	if (graphOrder_index == -1
		|| edges_index == -1)
	{
		FAIL("Saving view", "View does not include both graphOrder and edges. These are necessary to visualize the graphs.");
		return;
	}

	for (unsigned i = 0; i < number_rows; i++)
	{
		Graph g;
		g.read_graph_from_line(str_to_unsigned(&view_contents[i][graphOrder_index]), &view_contents[i][edges_index]);

		*file << g.convert_to_g6_format() << "\n";
	}
}


/**
* generates Macaulay2 scripts to compute the Betti tables for the factor ring of the initial ideal of the specified ideal type of all graphs satisfying query_condition, each labeled wrt to the given labeling
**/
void DatabaseInterface::generate_m2_scripts(std::string * idealname, unsigned * (Graph::*gen_labeling)(), unsigned batch_size, const char * query_condition, const char * filename, const char * labeling_name) {
	sqlite3_stmt * qry;

	if (query_condition)
	{
		if (sqlite3_prepare_v2(database, ("SELECT graphOrder,edges FROM Graphs WHERE " + std::string(query_condition)).c_str(), -1, &qry, 0) != SQLITE_OK)
		{
			FAIL("Generating M2 scripts", "SQL error: '" << qry << "' is an invalid query.");
			sqlite3_finalize(qry);
			return;
		}
	}
	else
	{
		if (sqlite3_prepare_v2(database, "SELECT graphOrder,edges FROM Graphs", -1, &qry, 0) != SQLITE_OK)
		{
			FAIL("Generating M2 scripts", "SQL error: '" << qry << "' is an invalid query.");
			sqlite3_finalize(qry);
			return;
		}
	}

	std::ifstream templ(filename ? filename : "template.m2");

	if (!templ.is_open())
	{
		FAIL("Generating M2 scripts", "Unable to open '" << (filename ? filename : "template.m2") << "'.");
		sqlite3_finalize(qry);
		return;
	}

	std::stringstream buffer;
	buffer << templ.rdbuf();
	templ.close();

	unsigned k;
	unsigned i;

	for (k = 0; true; k++)
	{
		std::string script = "G = {\n";
		std::string edges;
		std::string empty_edge_indices = "";

		for (i = 0; i < batch_size && sqlite3_step(qry) == SQLITE_ROW; i++)
		{
			edges = (char *)sqlite3_column_text(qry, 1) ? (char *)sqlite3_column_text(qry, 1) : "ERROR";
			Graph g(sqlite3_column_int(qry, 0), &edges);

			if (gen_labeling)
			{
				unsigned * labeling = (g.*gen_labeling)();
				script += g.convert_to_string_wrt_labeling(labeling) + ",\n";
				delete[] labeling;
			}
			else
				script += g.convert_to_string() + ",\n";

			if (edges == "{}")
				empty_edge_indices += std::to_string(i) + ",";
		}



		script.pop_back();
		script.pop_back();
		script += "\n}\n\nfilename=\"betti_" + *idealname + "_" + std::to_string(k) + ".bt\"\nN=" + std::to_string(i-1) + "\n\n" + buffer.str();

		std::ofstream kFile(*idealname + "_" + std::to_string(k) + ".m2", std::ios::trunc);

		if (!kFile.is_open())
		{
			FAIL("Generating M2 scripts", "Unable to write to file '" << *idealname << "_" << std::to_string(k) << ".m2'.");
			sqlite3_finalize(qry);
			return;
		}

		kFile << script;

		kFile.close();
		
		if (empty_edge_indices.empty())
			PROGRESS(2, "generated '" << *idealname << "_" << std::to_string(k) << ".m2'");
		else
		{
			empty_edge_indices.pop_back();
			PROGRESS(2, "generated '" << *idealname << "_" << std::to_string(k) << ".m2'");
			WARNING("Note, that there are no edges in graph(s) " << empty_edge_indices << " in this file. Taking the monomial ideal in this case might not work!");
		}

		if (i < batch_size)
			break;
	}

	sqlite3_finalize(qry);

	if (k == 0 && i == 0)
	{
		FAIL("Generating M2 scripts", "Unable to find any graphs satisfying the condition of the query: '" << qry << "'.");
		return;
	}


	std::string columns = "INSERT INTO Scripts (idealname,";
	std::string values = "date) VALUES ('" + *idealname + "',";

	if (labeling_name)
	{
		columns += "labeling,";
		values += "'" + std::string(labeling_name) + "',";
	}

	columns += "batchsize,";
	values += std::to_string(batch_size) + ",";

	if (query_condition)
	{
		columns += "condition,";
		values += "'" + std::string(query_condition) + "',";
	}

	values += "datetime())";

	execute_SQL_statement(&(columns + values));
}


/**
* outputs the status table to the terminal
**/
void DatabaseInterface::show_scripts() {
	sqlite3_stmt * stmt;

	if (sqlite3_prepare_v2(database, "SELECT * FROM Scripts", -1, &stmt, 0) != SQLITE_OK)
	{
		SQL_ERROR("SELECT * FROM Scripts");
		sqlite3_finalize(stmt);
		return;
	}

	unsigned colc = sqlite3_column_count(stmt);
	std::vector<std::string> columns;
	std::vector<unsigned> widths;

	for (unsigned i = 0; i < colc; i++)
	{
		std::string column = sqlite3_column_name(stmt, i);
		columns.push_back(column);
		widths.push_back(column.length() + 3);
	}

	std::vector<std::vector<std::string>> contents;
	unsigned rowc = 0;

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::vector<std::string> row;
		for (unsigned i = 0; i < colc; i++)
		{
			std::string content;
			content = (char *)sqlite3_column_text(stmt, i) ? (char *)sqlite3_column_text(stmt, i) : "-";

			row.push_back(content);

			if (widths[i] < content.length() + 3)
				widths[i] = content.length() + 3;
		}
		contents.push_back(row);
		rowc++;
	}

	sqlite3_finalize(stmt);

	std::cout << "\n";

	unsigned last_whitespace = 0;
	for (unsigned j = 0; j < colc; j++)
	{
		std::cout << std::string(last_whitespace, ' ') << columns[j];
		last_whitespace = widths[j] - columns[j].length();
	}
	std::cout << "\n";

	for (unsigned i = 0; i < rowc; i++)
	{
		last_whitespace = 0;

		for (unsigned j = 0; j < colc; j++)
		{
			std::string content = contents[i][j];
			std::cout << std::string(last_whitespace, ' ') << content;
			last_whitespace = widths[j] - content.length();
		}
		std::cout << "\n";
	}

	std::cout << "\n" << std::endl;
}


/**
 * expects a SELECT SQL-statement
 * tries to execute query, save the result and output the view to the terminal
**/
bool DatabaseInterface::execute_SQL_query(std::string * query) {
	if (query->empty())
		return true;

	sqlite3_stmt * stmt;

	try
	{
		if (sqlite3_prepare_v2(database, query->c_str(), -1, &stmt, 0) != SQLITE_OK)
		{
			SQL_ERROR(*query);
			sqlite3_finalize(stmt);
			return false;
		}

		reset_view();

		number_columns = sqlite3_column_count(stmt);

		for (unsigned i = 0; i < number_columns; i++)
		{
			std::string column = sqlite3_column_name(stmt, i);
			view_columns.push_back(column);
			column_widths.push_back(column.length() + 3);
		}

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::vector<std::string> row;
			for (unsigned i = 0; i < number_columns; i++)
			{
				std::string content = (char *)sqlite3_column_text(stmt, i) ? (char *)sqlite3_column_text(stmt, i) : "-";
				row.push_back(content);

				if (column_widths[i] < content.length() + 3)
					column_widths[i] = content.length() + 3;
			}
			view_contents.push_back(row);
			number_rows++;
		}

		sqlite3_finalize(stmt);

		show_view();
	}
	catch (const char * error)
	{
		sqlite3_finalize(stmt);
		reset_view();
		FAIL("SQL query", error << ". Resetting view.");
		return false;
	}

	return true;
}


/**
 * tries to execute given statement without changing the current view
**/
bool DatabaseInterface::execute_SQL_statement(std::string * statement) {
	char * errMsg = 0;

	if (sqlite3_exec(database, statement->c_str(), 0, 0, &errMsg) != SQLITE_OK) {
		SQL_ERROR(*statement);
		RESULT(errMsg);
		sqlite3_free(errMsg);
		return false;
	}

	return true;
}


/**
* creates the script table in database for ensuring database consistency
**/
bool DatabaseInterface::create_scripts_table() {
	std::string statement = "CREATE TABLE Scripts(" \
		"scriptID INTEGER PRIMARY KEY," \
		"idealname TEXT NOT NULL," \
		"labeling TEXT," \
		"batchsize INT NOT NULL," \
		"condition TEXT," \
		"date TEXT NOT NULL" \
		");";

	if (sqlite3_exec(database, statement.c_str(), 0, 0, 0) != SQLITE_OK)
		return false;

	return true;
}


/**
 * creates graphs table in database
**/
bool DatabaseInterface::create_graphs_table() {
	std::string statement = "CREATE TABLE IF NOT EXISTS Graphs("  \
							"graphID INTEGER PRIMARY KEY," \
							"graphOrder INT NOT NULL," \
							"graphSize INT NOT NULL," \
							"edges TEXT NOT NULL," \
							"cliqueNumber INT," \
							"maxCliqueNumber INT," \
							"type TEXT" \
							");";

	return execute_SQL_statement(&statement);
}


/**
 * batch inserts graphs from given file
**/
void DatabaseInterface::insert_graphs(std::ifstream * file, FORMAT format) {
	if (format == FORMAT::NONE)
		throw "noFormat";

	sqlite3_exec(database, "BEGIN TRANSACTION;", 0, 0, 0);

	bool (Graph::*read_function)(std::ifstream *) = &Graph::read_next_g6_format;
	
	if (format == FORMAT::LIST)
		read_function = &Graph::read_next_list_format;
	
	while (true)
	{
		unsigned i;
		Graph g;
		std::string statement = "INSERT INTO Graphs (graphOrder,graphSize,edges,cliqueNumber,maxCliqueNumber) VALUES ";

		for (i = 0; i < 10000 && (g.*read_function)(file); i++)
		{
			std::pair<unsigned, unsigned> clique_numbers = g.get_clique_numbers();
			statement += "(" + std::to_string(g.get_order()) + "," + std::to_string(g.get_size()) + ",'" + g.convert_to_string() + "'," + std::to_string(clique_numbers.first) + "," + std::to_string(clique_numbers.second) + "),";//TEST
		}

		statement.pop_back();

		execute_SQL_statement(&statement);

		PROGRESS(2, i << " graphs inserted");

		if (i < 10000)
			break;
	}

	sqlite3_exec(database, "COMMIT;", 0, 0, 0);
}


/**
 * updates the type of all graphs that satisfy graph_test and query_condition
**/
bool DatabaseInterface::update_type(bool(Graph::*graph_test)(), const char * type, const char * query_condition) {
	std::string query = "SELECT graphID,graphorder,edges,type FROM Graphs WHERE (type IS NULL OR type NOT LIKE '%" + std::string(type) + "%')";
	if (query_condition)
		query += " AND " + std::string(query_condition);

	sqlite3_stmt * qry;

	if (sqlite3_prepare_v2(database, query.c_str(), -1, &qry, 0) != SQLITE_OK)
	{
		SQL_ERROR(query);
		FAIL("Labeling type", "");
		sqlite3_finalize(qry);
		return false;
	}

	sqlite3_stmt * stmt;

	if (sqlite3_prepare_v2(database, "UPDATE Graphs SET type = ? WHERE graphID == ?", -1, &stmt, 0) != SQLITE_OK)
	{
		SQL_ERROR("UPDATE Graphs SET type = ? WHERE graphID == ?");
		FAIL("Labeling type", "");
		sqlite3_finalize(qry);
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_exec(database, "BEGIN TRANSACTION;", 0, 0, 0);
	unsigned i;
	for (i = 1; sqlite3_step(qry) == SQLITE_ROW; i++)
	{
		std::string edges = ((char *)sqlite3_column_text(qry, 2));
		Graph g(sqlite3_column_int(qry, 1), &edges);

		if ((g.*graph_test)())
		{
			sqlite3_bind_text(stmt, 1, sqlite3_column_text(qry, 3) ? (std::string((char *)sqlite3_column_text(qry, 3)) + "," + std::string(type)).c_str() : type, -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 2, sqlite3_column_int(qry, 0));

			sqlite3_step(stmt);

			sqlite3_clear_bindings(stmt);
			sqlite3_reset(stmt);
		}

		if (i == 10000)
		{
			PROGRESS(2, i << " graphs tested");
			i = 1;
		}
	}
	sqlite3_exec(database, "COMMIT;", 0, 0, 0);

	sqlite3_finalize(qry);
	sqlite3_finalize(stmt);

	if (i > 1)
		PROGRESS(2, i - 1 << " graphs tested");
	else
	{
		FAIL("Labeling type", "Unable to find any graphs satisfying the condition of the query: '" << query << "'.");
		return false;
	}

	return true;
}


/**
 * reads all Betti tables from the files determined by the given order and ideal_type, updates the graphs table and all graphs of given order
**/
bool DatabaseInterface::add_betti_data(unsigned scriptID) {
	sqlite3_stmt * qry1;
	if (sqlite3_prepare_v2(database, ("SELECT idealname,condition FROM Scripts WHERE scriptID == " + std::to_string(scriptID)).c_str(), -1, &qry1, 0) != SQLITE_OK)
	{
		SQL_ERROR("SELECT idealname,condition FROM Scripts WHERE scriptID == " << scriptID);
		FAIL("Adding Betti data", "");
		sqlite3_finalize(qry1);
		return false;
	}

	if (sqlite3_step(qry1) != SQLITE_ROW)
	{
		FAIL("Adding Betti data", "There is no open script with scriptID " << scriptID << " in the Scripts table.");
		sqlite3_finalize(qry1);
		return false;
	}

	std::string ideal = (char *)sqlite3_column_text(qry1, 0);
	std::string query_condition = (char *)sqlite3_column_text(qry1, 1) ? (char *)sqlite3_column_text(qry1, 1) : "";
	sqlite3_finalize(qry1);


	size_t cut_index = 0;
	while (cut_index != std::string::npos)
	{
		cut_index = ideal.find_first_of(' ');
		if (cut_index < ideal.length() - 1)
			ideal = ideal.substr(0, cut_index) + ideal.substr(cut_index + 1, std::string::npos);
		else if (cut_index == ideal.length() - 1)
			ideal.pop_back();
	}

	sqlite3_exec(database, (std::string("ALTER TABLE Graphs ADD ") + ideal + "Bettis TEXT;").c_str(), 0, 0, 0);
	sqlite3_exec(database, (std::string("ALTER TABLE Graphs ADD ") + ideal + "PD INT;").c_str(), 0, 0, 0);
	sqlite3_exec(database, (std::string("ALTER TABLE Graphs ADD ") + ideal + "Reg INT;").c_str(), 0, 0, 0);
	sqlite3_exec(database, (std::string("ALTER TABLE Graphs ADD ") + ideal + "Schenzel INT;").c_str(), 0, 0, 0);
	sqlite3_exec(database, (std::string("ALTER TABLE Graphs ADD ") + ideal + "Extremals TEXT;").c_str(), 0, 0, 0);


	sqlite3_stmt * qry2;
	if (query_condition.empty())
	{
		if (sqlite3_prepare_v2(database, "SELECT graphID FROM Graphs", -1, &qry2, 0) != SQLITE_OK)
		{
			SQL_ERROR("SELECT graphID FROM Graphs");
			FAIL("Adding Betti data", "");
			sqlite3_finalize(qry2);
			return false;
		}
	}
	else
	{
		if (sqlite3_prepare_v2(database, ("SELECT graphID FROM Graphs WHERE " + std::string(query_condition)).c_str(), -1, &qry2, 0) != SQLITE_OK)
		{
			SQL_ERROR("SELECT graphID FROM Graphs" << query_condition);
			FAIL("Adding Betti data", "");
			sqlite3_finalize(qry2);
			return false;
		}
	}
	

	std::string statement = "UPDATE Graphs SET " + ideal + "Bettis = ?, " + ideal + "PD = ?, " + ideal + "Reg = ?, " + ideal + "Schenzel = ?, " + ideal + "Extremals = ? WHERE graphID == ?;";
	sqlite3_stmt * stmt1;

	if (sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt1, 0) != SQLITE_OK)
	{
		SQL_ERROR(statement);
		FAIL("Adding Betti data", "");
		sqlite3_finalize(qry2);
		sqlite3_finalize(stmt1);
		return false;
	}

	sqlite3_exec(database, "BEGIN TRANSACTION;", 0, 0, 0);
	unsigned count = 0;
	for (unsigned k = 0; true; k++)
	{
		std::ifstream kFile("betti_" + ideal + "_" + std::to_string(k) + ".bt");

		if (!kFile.is_open())
		{
			if (k == 0)
			{
				FAIL("Adding Betti data", "Unable to open 'betti_" << ideal << "_0.bt'.");
				sqlite3_exec(database, "COMMIT;", 0, 0, 0);
				sqlite3_finalize(qry2);
				sqlite3_finalize(stmt1);
				return false;
			}
			break;
		}

		BettiTable b;

		for (unsigned i = 1; b.read_next_table(&kFile); i++)
		{
			if (sqlite3_step(qry2) != SQLITE_ROW)
			{
				FAIL("Adding Betti data", "There are more tables in the files than graphs of order satisfying condition '" << query_condition << "' in the database.");
				sqlite3_exec(database, "COMMIT;", 0, 0, 0);
				sqlite3_finalize(qry2);
				sqlite3_finalize(stmt1);
				return false;
			}

			sqlite3_bind_text(stmt1, 1, b.convert_to_line().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt1, 2, b.get_projective_dimension());
			sqlite3_bind_int(stmt1, 3, b.get_regularity());
			sqlite3_bind_int(stmt1, 4, b.get_schenzel_number());
			sqlite3_bind_text(stmt1, 5, b.get_extremal_betti_numbers_as_string().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt1, 6, sqlite3_column_int(qry2, 0));

			sqlite3_step(stmt1);

			sqlite3_clear_bindings(stmt1);
			sqlite3_reset(stmt1);

			count++;

			if (count == 10000)
			{
				PROGRESS(2, count << " graphs updated");
				count = 0;
			}
		}
	}
	if (count != 0)
		PROGRESS(2, count << " graphs updated");
	sqlite3_exec(database, "COMMIT;", 0, 0, 0);

	sqlite3_finalize(qry2);
	sqlite3_finalize(stmt1);

	execute_SQL_statement(&("DELETE FROM Scripts WHERE scriptID == " + std::to_string(scriptID)));

	return true;
}
