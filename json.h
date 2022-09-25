#ifndef JSON_H
#define JSON_H
#pragma pack(1)
typedef struct json {
  int id;
  char name[32];
  int amount;
  int price;
} json;
#pragma pack(0)
#endif /* JSON_H */
