#ifndef LIST_H_WYVXF5HO
#define LIST_H_WYVXF5HO

#define NULL_NODE (node_t *)0x0

typedef struct _node_t {
  struct _node_t *prev;
  struct _node_t *next;
  void           *value;
  void           *owner; // WHAT IS THIS?
} __attribute__((packed)) node_t;

typedef struct _list_t {
  node_t *head;
  node_t *tail;
  size_t size;
} __attribute__((packed)) list_t;

list_t *    list_init           (void);
void        list_free           (list_t *list);
void        list_destroy        (list_t *list);

node_t *    list_insert         (list_t *list, node_t *node);
node_t *    list_insert_at      (list_t *list, size_t index);

void        list_remove         (list_t *list, node_t *node);
void        list_remove_at      (list_t *list, size_t index);

node_t *    list_node_at        (list_t *list, size_t index);
size_t      list_index_of       (list_t *list, node_t *node);

node_t *    list_find_node      (list_t *list, void *value);
size_t      list_find_index     (list_t *list, void *value);

#define list_foreach(item, list) \
    for (node_t *item = list->head; item->next != NULL_NODE; item = item->next)

#define list_foreach_rev(item, list) \
    for (node_t *item = list->tail; item->prev != NULL_NODE; item = item->prev)

#endif /* end of include guard: LIST_H_WYVXF5HO */
