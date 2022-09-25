#include "transferfcns.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CANT_RECEIVE -1
#define BROKEN_PACKET -2
#define CANT_SEND -3

int get_client_info_msg(int sock, request_msg *message)
{
  int recv_bytes;
  if ((recv_bytes = recv(sock, message, sizeof(*message), 0)) < 1)
  {
    perror("Error get_client_info_msg: can\'t receive\n");
    return CANT_SEND;
  }
  if (recv_bytes != sizeof(*message))
  {
    perror("Error get_client_info_msg: received bytes != sizeof(message)!\n");
    return BROKEN_PACKET;
  }

  return recv_bytes;
}

int send_client_info_msg(int sock, int goods_size)
{
  int send_bytes;
  request_msg message;
  message.id = 1;
  message.amount = goods_size;

  if ((send_bytes = send(sock, &message, sizeof(message), 0)) < 1)
  {
    perror("Error: send_client_info_msg: can\'t send\n");
    return CANT_SEND;
  }

  return send_bytes;
}

int send_goods_list(int sock, json goods_list[], int goods_size)
{
  int send_sum = 0;
  int send_bytes;

  for (int i = 0; i < goods_size; i++)
  {
    if ((send_bytes = send(sock, &goods_list[i], sizeof(goods_list[i]), 0)) <
        1)
    {
      perror("Error: send_goods_list: can\'t send\n");
      return CANT_SEND;
    }
    send_sum += send_bytes;
  }

  return send_sum;
}

int send_request_and_get_response(int sockfd, request_msg *message)
{
  int numbytes;
  if ((numbytes = send(sockfd, message, sizeof(*message), 0)) <= 0)
  {
    perror("Error send_request_and_get_response: can\'t send\n");
    return CANT_SEND;
  }

  if (recv(sockfd, &numbytes, sizeof(numbytes), 0) <= 0)
  {
    numbytes = -1;
  }

  return numbytes;
}

int get_list_of_goods(int sock, json goods[], int list_size)
{
  int recv_bytes;
  int recv_sum = 0;

  for (int i = 0; i < list_size; i++)
  {
    if ((recv_bytes = recv(sock, &goods[i], sizeof(goods[i]), 0)) <= 0)
    {
      perror("Can\'t receive\n");
      return CANT_RECEIVE;
    }
    if (recv_bytes != sizeof(goods[i]))
    {
      printf("Packet #%d: ", i);
      perror("Error get_list_of_goods: received bytes != sizeof(message)!\n");
      return BROKEN_PACKET;
    }
    recv_sum += recv_bytes;
  }

  return recv_sum;
}
