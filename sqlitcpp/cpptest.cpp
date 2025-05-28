#include <stdio.h>
//#include <sqlite3.h> 
#include "sqldb.hpp"

int main(int argc, char* argv[]) {

   char *zErrMsg = 0;
   int rc;
   char dbfile[] = "test.db";
   SqlDb mydb;
   
   mydb.db_open(dbfile);
   mydb.db_create_table();
   mydb.db_close();


}
