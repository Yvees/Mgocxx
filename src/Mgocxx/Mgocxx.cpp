// Mgocxx.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <bson.h>
#include <iostream>
#include <thread>

using namespace std;

int _finish = 0;
int thread_num = 128;		//线程数
void output()
{
	mongoc_init();

	mongoc_client_t * client = mongoc_client_new("mongodb://10.99.66.44:27017/?appname=insert-example");
	mongoc_collection_t * collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	for (size_t i = 0; i < 100000; i++)
	{
		bson_error_t error;
		bson_oid_t oid;
		bson_t *doc = bson_new();
		bson_oid_init(&oid, NULL);
		BSON_APPEND_OID(doc, "_id", &oid);
		BSON_APPEND_UTF8(doc, "hello", "world");

		if (!mongoc_collection_insert_one(
			collection, doc, NULL, NULL, &error)) {
			fprintf(stderr, "%s\n", error.message);
		}

		bson_destroy(doc);
	}

	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	_finish++;
}

int main(int argc, char *argv[])
{

	time_t start = time(NULL);

	for (uint8_t i = 0; i < thread_num; i++)
	{
		thread t(output);
		t.detach();
	}

	while (true)
	{
		if (_finish >= thread_num)
		{
			break;
		}
	}


	time_t end = time(NULL);
	cout << "finish write." << (end - start);

	mongoc_cleanup();

	getchar();
	return 0;
}

