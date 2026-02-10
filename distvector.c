#include <stdio.h>
#include <stdlib.h>

struct Node {
  int dist[50];
  int from[50];
};

void main() {
  int no;
  printf("Enter the number of nodes: ");
  scanf("%d", &no);

  int dm[no][no], i, j;
  struct Node route[no];
  printf("Enter the distance matrix: \n");
  for (i = 0; i < no; i++)
    for (j = 0; j < no; j++) {
      scanf("%d", &dm[i][j]);
      if (i == j)
        dm[i][j] = 0;
      route[i].dist[j] = dm[i][j];
      route[i].from[j] = j;
    }

  int flag;
  do {
    flag = 0;
    for (i = 0; i < no; i++)
      for (j = 0; j < no; j++)
        for (int k = 0; k < no; k++) {
          if (route[i].dist[j] < route[i].dist[k] + route[k].dist[j]) {
            route[i].dist[j] = route[i].dist[k] + route[k].dist[j];
            route[i].from[j] = route[i].from[k];
            flag = 1;
          }
        }
  } while (flag);

  for (i = 0; i < no; i++) {
    printf("Router info for Router: %c\n", i + 65);
    printf("Destination\tNext-Hop\tDistance\n");
    for (j = 0; j < no; j++) {
      printf("%c\t\t%c\t\t%d\n", j + 65, route[i].from[j] + 65,
             route[i].dist[j]);
    }
  }
}
