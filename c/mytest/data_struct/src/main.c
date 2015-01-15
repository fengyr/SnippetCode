/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  四月 11, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "list.h"
#include "hashtable.h"
#include "hashmap.h"
#include "object.h"
#include "rb-tree.h"

/* #define TEST_ARRAY */
/* #define TEST_LIST */
/* #define TEST_HASHTABLE */
/* #define TEST_HASHMAP */
#define TEST_RBTREE

struct object_array g_object_array; 
struct list_t g_List;
struct hash_table_t g_hashtable;

static unsigned int hashpjw(const void *key)
{
    const char *ptr;
    unsigned int val;
    
    val = 0;
    ptr = (const char*) key;

    while (*ptr != '\0') {
        unsigned int tmp;
        val = (val << 4) + (*ptr);

        if ((tmp = (val & 0xf0000000))) {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }

        ptr++;
    }

    return val % 10240;
}

static void dump_object_array(ArrayObj *array)
{
    ArrayEntry *objects = array->objects; 
    struct object *obj;
    int i = 0;

    printf("nr=%d, alloc=%d\n", array->nr, array->alloc);
    for (i = 0; i < array->nr; i++) {
        obj = (struct object*) objects[i].item;
        printf("object id=%d, name=%s\n", obj->id, obj->name);
    }
}

static void list_dump(List *list)
{
    ListElmt *element = list->head;

    if (list_size(list) == 0) {
        printf("list dump == NULL\n");
    } else {
        while (element != NULL) {
            struct object *data = (struct object*)element->data;
            printf("list dump name=%s, id=%d\n", data->name, data->id);

            element = element->next;
        }
    }
}

void test_object_array(int size)
{
    ArrayObj *obj_array = &g_object_array;
    obj_array->nr = 0;
    obj_array->alloc = 0;
    obj_array->objects = NULL;
    obj_array->destroy = free_object;

    /* 添加数据前，打印数组信息 */
    dump_object_array(obj_array); 

    int i = 0;
    char obj_name[16];

    for (i = 0; i < size; i++) {
        sprintf(obj_name, "object:%d", i);
        struct object *obj = (struct object*)create_object(obj_name, i);

        array_obj_add((void*)obj, obj_array);
    }
    /* 添加数据后，打印数组信息 */
    dump_object_array(obj_array);    

    /* 释放数组内存,打印数组信息 */
    array_obj_destroy(obj_array);
    dump_object_array(obj_array);
}

void test_list(int size)
{
    List *list = &g_List;    
    list_init(list, free_object);
    int i = 0;
    char obj_name[16];
    char array_name[16];

    for (i = 0; i < size; i++) {
        sprintf(array_name, "array:%d", i);
        sprintf(obj_name, "object:%d", i);
        struct object *obj = (struct object*)create_object(obj_name, i);
        list_insert_next(list, NULL, obj);
    }

    list_dump(list);

    list_destroy(list);
    list_dump(list);
}

void test_hashtable(int size)
{
    HashTable *htbl = &g_hashtable;
    hashtbl_init(htbl, 10, hashpjw, NULL, free_object);

    int i = 0;
    int retval;
    char key[128];
    
    for (i = 0; i < size; i++) {
        memset(key, 0, 128);
        sprintf(key, "key:%d", i);
        hashtbl_insert(htbl, key);
    }

    printf("hashtable size: %d\n", hashtbl_size(htbl));

    for (i = 0; i < size; i++) {
        memset(key, 0, 128);
        sprintf(key, "key:%d", i);
        retval = hashtbl_lookup(htbl, (void**)key);
        printf("hashtable lookup: %d\n", retval);
    }

    hashtbl_destroy(htbl);
}

void test_hashmap(int size)
{
#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (10)

    typedef struct data_struct_s
    {
        char key_string[KEY_MAX_LENGTH];
        int number;
    } data_struct_t;

    int index;
    int error;
    HashMap mymap;
    char key_string[KEY_MAX_LENGTH];
    data_struct_t* value;
    
    mymap = hashmap_new();

    // 初始化一组hashmap值
    for (index=0; index<KEY_COUNT; index+=1)
    {
        /* Store the key string along side the numerical value so we can free it later */
        value = (data_struct_t*)malloc(sizeof(data_struct_t));
        snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
        value->number = index;

        error = hashmap_put(mymap, value->key_string, value);
        assert(error==MAP_OK);
        printf("put: key=%s, val_string=%s, val_num=%d\n", value->key_string, value->key_string, value->number);
    }

    printf("====================================\n");

    // 检查初始化值是否正确
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        
        /* Make sure the value was both found and the correct number */
        assert(error==MAP_OK);
        assert(value->number==index);
        printf("get: key=%s, val_string=%s, val_num=%d\n", key_string, value->key_string, value->number);
    }
    
    /* Make sure that a value that wasn't in the map can't be found */
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);

    error = hashmap_get(mymap, key_string, (void**)(&value));
        
    /* Make sure the value was not found */
    assert(error==MAP_MISSING);

    /* Free all of the values we allocated and remove them from the map */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        assert(error==MAP_OK);

        error = hashmap_remove(mymap, key_string);
        assert(error==MAP_OK);

        free(value);        
    }
    
    /* Now, destroy the map */
    hashmap_free(mymap);
}

void test_rb_tree_new();
void test_rb_tree_free();
void test_rb_tree_child();
void test_rb_tree_insert_lookup();
void test_rb_tree_lookup();
void test_out_of_memory();

static int int_compare(void *vlocation1, void *vlocation2)
{
	int *location1;
	int *location2;

	location1 = (int *) vlocation1;
	location2 = (int *) vlocation2;

	if (*location1 < *location2) {
		return -1;
	} else if (*location1 > *location2) {
		return 1;
	} else {
		return 0;
	}
}

void test_rb_tree()
{
    test_rb_tree_new();
	test_rb_tree_free();
	test_rb_tree_child();
	test_rb_tree_insert_lookup();
	test_rb_tree_lookup();
	test_out_of_memory();
    
    RBTree *tree;
	RBTreeNode *node;
	void *key_val, *val_val;
    int NUM_TEST_VALUES = 1000000;
	int values[NUM_TEST_VALUES];
    int i;
    for (i = 0; i < NUM_TEST_VALUES; i++) {
        values[i] = i;
    }

    // 创建树
	tree = rb_tree_new((RBTreeCompareFunc) int_compare);

    // 插入数据
	for (i=0; i<NUM_TEST_VALUES; ++i) {
		rb_tree_insert(tree, &values[i], &values[i]);
	}

    // 查找数据
    for (i=0; i<NUM_TEST_VALUES; ++i) {
		node = rb_tree_lookup_node(tree, &i);
		assert(node != NULL);
		key_val = rb_tree_node_key(node);
		val_val = rb_tree_node_value(node);
		printf("%02d, key=%d, val=%d\n", i, *((int*)key_val), *((int*)val_val));
	}

    // 查找错误数据
    int key = -1;
	node = rb_tree_lookup_node(tree, &key);
    printf("key error, node=%p\n", node);

	rb_tree_free(tree);
}

int main(int argc, const char *argv[])
{
#ifdef TEST_ARRAY
    /* 测试object数组 */
    test_object_array(100000);
    test_object_array(10000);
    test_object_array(10);
    test_object_array(100);
    test_object_array(1000);
#endif

#ifdef TEST_LIST
     /* 测试list链表 */
    /* test_list(1000); */
    /* test_list(100); */
    test_list(10);
    /* test_list(100); */
    /* test_list(1000); */
#endif

#ifdef TEST_HASHTABLE
    test_hashtable(10);
#endif

#ifdef TEST_HASHMAP
    test_hashmap(10);
#endif

#ifdef TEST_RBTREE
    test_rb_tree(); 
#endif

    
    return 0;
}
