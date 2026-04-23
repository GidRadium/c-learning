#include "map.h"
#include <stdlib.h>

typedef struct MapNode {
    MapKey key;
    MapValue value;
    int height;
    struct MapNode* left;
    struct MapNode* right;
} MapNode;

typedef struct Map {
    MapNode* root;
    size_t size;
    MapValueCopyFunc copyFunc;
    MapValueFreeFunc freeFunc;
} Map;

int getHeight(MapNode* node)
{
    if (node == NULL) {
        return 0;
    }

    return node->height;
}

int getBalance(MapNode* node)
{
    if (node == NULL) {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(MapNode* node)
{
    if (node == NULL) {
        return;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

MapNode* rotateRight(MapNode* pivot)
{
    MapNode* newRoot = pivot->left;
    MapNode* middleSubtree = newRoot->right;

    newRoot->right = pivot;
    pivot->left = middleSubtree;

    updateHeight(pivot);
    updateHeight(newRoot);

    return newRoot;
}

MapNode* rotateLeft(MapNode* pivot)
{
    MapNode* newRoot = pivot->right;
    MapNode* middleSubtree = newRoot->left;

    newRoot->left = pivot;
    pivot->right = middleSubtree;

    updateHeight(pivot);
    updateHeight(newRoot);

    return newRoot;
}

MapNode* rebalance(MapNode* node)
{
    updateHeight(node);
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }

    // Left Right
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Right
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }

    // Right Left
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

MapNode* nodeFind(MapNode* node, MapKey key)
{
    while (node != NULL) {
        if (key == node->key) {
            return node;
        }

        if (key < node->key) {
            node = node->left;
            continue;
        }

        node = node->right;
    }

    return NULL;
}

// Only if node not in tree
MapNode* nodeInsert(MapNode* node, MapNode* newNode)
{
    if (node == NULL) {
        return newNode;
    }

    if (newNode->key < node->key) {
        node->left = nodeInsert(node->left, newNode);
    } else if (newNode->key > node->key) {
        node->right = nodeInsert(node->right, newNode);
    }

    return rebalance(node);
}

MapNode* nodeCreate(MapKey key, MapValue valueDeepCopy)
{
    MapNode* node = malloc(sizeof(MapNode));
    if (node == NULL) {
        return NULL;
    }

    node->key = key;
    node->value = valueDeepCopy;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

MapValue nodeDelete(MapNode** nodeRef, MapKey key)
{
    MapNode* node = *nodeRef;
    MapValue valueToFree = NULL;

    if (key < node->key) {
        valueToFree = nodeDelete(&node->left, key);
    } else if (key > node->key) {
        valueToFree = nodeDelete(&node->right, key);
    } else {
        valueToFree = node->value;
        if (node->left == NULL || node->right == NULL) {
            MapNode* temp = node->left != NULL ? node->left : node->right;
            *nodeRef = temp;

            free(node);
        } else {
            MapNode* successor = node->right;

            while (successor != NULL && successor->left != NULL) {
                successor = successor->left;
            }

            node->key = successor->key;
            node->value = successor->value;

            valueToFree = nodeDelete(&node->right, successor->key);
        }
    }

    if (*nodeRef != NULL) {
        *nodeRef = rebalance(*nodeRef);
    }

    return valueToFree;
}

void freeTree(MapNode* node, MapValueFreeFunc freeFunc)
{
    if (node == NULL) {
        return;
    }

    freeTree(node->left, freeFunc);
    freeTree(node->right, freeFunc);
    freeFunc(node->value);

    free(node);
}

MapReturnCode mapCreate(Map** map, MapValueCopyFunc copyFunc, MapValueFreeFunc freeFunc)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    *map = malloc(sizeof(Map));
    if (*map == NULL) {
        return MapErrOnMalloc;
    }

    (*map)->root = NULL;
    (*map)->size = 0;
    (*map)->copyFunc = copyFunc;
    (*map)->freeFunc = freeFunc;

    return MapSucsess;
}

MapReturnCode mapSet(Map* map, MapKey key, MapValue value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    MapNode* existingNode = nodeFind(map->root, key);

    MapValue newValue = map->copyFunc(value);
    if (newValue == NULL && value != NULL) {
        return MapErrOnMalloc;
    }

    if (existingNode != NULL) {
        map->freeFunc(existingNode->value);
        existingNode->value = newValue;

        return MapSucsess;
    }

    MapNode* newNode = nodeCreate(key, newValue);
    if (newNode == NULL) {
        map->freeFunc(newValue);

        return MapErrOnMalloc;
    }

    map->root = nodeInsert(map->root, newNode);
    map->size++;

    return MapSucsess;
}

MapReturnCode mapGet(Map* map, MapKey key, MapValue* value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    MapNode* node = nodeFind(map->root, key);
    if (node == NULL) {
        return MapErrKeyNotFound;
    }

    if (value != NULL) {
        *value = node->value;
    }

    return MapSucsess;
}

MapReturnCode mapContains(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    if (nodeFind(map->root, key) == NULL) {
        return MapErrKeyNotFound;
    }

    return MapSucsess;
}

MapReturnCode mapErase(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    if (nodeFind(map->root, key) == NULL) {
        return MapErrKeyNotFound;
    }

    MapValue valueToFree = nodeDelete(&map->root, key);
    map->freeFunc(valueToFree);
    map->size--;

    return MapSucsess;
}

MapReturnCode mapGetSize(Map* map, size_t* mapSize)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    if (mapSize != NULL) {
        *mapSize = map->size;
    }

    return MapSucsess;
}

MapReturnCode mapDelete(Map** map)
{
    if (map == NULL || (*map) == NULL) {
        return MapErrNoMap;
    }

    freeTree((*map)->root, (*map)->freeFunc);
    free(*map);
    *map = NULL;

    return MapSucsess;
}

struct Iterator {
    MapNode** stack;
    size_t stackSize;
    size_t stackCapacity;
};

static bool pushLeftBranch(Iterator* it, MapNode* node)
{
    while (node != NULL) {
        if (it->stackSize >= it->stackCapacity) {
            size_t newCapacity = it->stackCapacity == 0 ? 8 : it->stackCapacity * 2;
            MapNode** newStack = realloc(it->stack, newCapacity * sizeof(MapNode*));
            if (newStack == NULL) {
                return false;
            }

            it->stack = newStack;
            it->stackCapacity = newCapacity;
        }

        it->stack[it->stackSize] = node;
        it->stackSize = it->stackSize + 1;

        node = node->left;
    }

    return true;
}

Iterator* iteratorInit(Map* map)
{
    if (map == NULL) {
        return NULL;
    }

    Iterator* it = malloc(sizeof(Iterator));
    if (it == NULL) {
        return NULL;
    }

    it->stack = NULL;
    it->stackSize = 0;
    it->stackCapacity = 0;

    if (!pushLeftBranch(it, map->root)) {
        free(it->stack);
        free(it);
        return NULL;
    }

    return it;
}

bool iteratorHasNext(Iterator* it)
{
    if (it == NULL) {
        return false;
    }

    return it->stackSize > 0;
}

MapEntry iteratorNext(Iterator* it)
{
    MapEntry entry = { 0 };

    if (it == NULL || it->stackSize == 0) {
        return entry;
    }

    it->stackSize = it->stackSize - 1;
    MapNode* node = it->stack[it->stackSize];

    entry.key = node->key;
    entry.value = node->value;

    pushLeftBranch(it, node->right);

    return entry;
}

void iteratorDelete(Iterator* it)
{
    if (it == NULL) {
        return;
    }

    free(it->stack);
    free(it);
}
