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

MapNode* findNode(MapNode* node, MapKey key)
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
MapNode* insertNode(MapNode* node, MapNode* newNode)
{
    if (node == NULL) {
        return newNode;
    }

    if (newNode->key < node->key) {
        node->left = insertNode(node->left, newNode);
    } else if (newNode->key > node->key) {
        node->right = insertNode(node->right, newNode);
    }

    return rebalance(node);
}

MapNode* createNode(MapKey key, MapValue valueDeepCopy)
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

    MapNode* existingNode = findNode(map->root, key);

    MapValue newValue = map->copyFunc(value);
    if (newValue == NULL && value != NULL) {
        return MapErrOnMalloc;
    }

    if (existingNode != NULL) {
        map->freeFunc(existingNode->value);
        existingNode->value = newValue;

        return MapSucsess;
    }

    MapNode* newNode = createNode(key, newValue);
    if (newNode == NULL) {
        map->freeFunc(newValue);

        return MapErrOnMalloc;
    }

    map->root = insertNode(map->root, newNode);
    map->size++;

    return MapSucsess;
}

MapReturnCode mapGet(Map* map, MapKey key, MapValue* value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    MapNode* node = findNode(map->root, key);
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

    if (findNode(map->root, key) == NULL) {
        return MapErrKeyNotFound;
    }

    return MapSucsess;
}

MapReturnCode mapErase(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

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

    return MapSucsess;
}
