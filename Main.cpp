#include"game.hpp"

int main()
{
    pthread_t id1;
    pthread_create(&id1, NULL, game, NULL);
    pthread_join(id1, NULL);
    return 0;
}