#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_SIZE 4
#define TOTAL_PACKETS 10

typedef struct
{
    int seq_no;
    bool acked;
} Packet;

int main()
{
    Packet sender_buffer[TOTAL_PACKETS];
    Packet receiver_buffer[TOTAL_PACKETS];

    // Initialize packets in sender buffer
    for (int i = 0; i < TOTAL_PACKETS; i++)
    {
        sender_buffer[i].seq_no = i;
        sender_buffer[i].acked = false;
    }

    // Initialize receiver buffer
    for (int i = 0; i < TOTAL_PACKETS; i++)
    {
        receiver_buffer[i].seq_no = -1;
        receiver_buffer[i].acked = false;
    }

    int sender_base = 0;
    int receiver_base = 0;

    printf("Sender buffer (initial):\n");
    for (int i = 0; i < TOTAL_PACKETS; i++)
    {
        printf("Seq No: %d, Acked: %s\n", sender_buffer[i].seq_no, sender_buffer[i].acked ? "true" : "false");
    }
    printf("\n");

    while (receiver_base < TOTAL_PACKETS)
    {
        // Sender side
        for (int i = sender_base; i < sender_base + WINDOW_SIZE && i < TOTAL_PACKETS; i++)
        {
            if (!sender_buffer[i].acked)
            {
                printf("Sending packet with seq no: %d\n", sender_buffer[i].seq_no);
            }
        }

        // Receiver side
        for (int i = receiver_base; i < receiver_base + WINDOW_SIZE && i < TOTAL_PACKETS; i++)
        {
            if (receiver_buffer[i].seq_no == -1)
            {
                printf("Receiver: Received packet with seq no: %d\n", i);
                receiver_buffer[i].seq_no = i;
                receiver_buffer[i].acked = true;
            }
        }

        // Move sender window
        while (sender_base < TOTAL_PACKETS && sender_buffer[sender_base].acked)
        {
            sender_base++;
        }

        // Move receiver window
        while (receiver_base < TOTAL_PACKETS && receiver_buffer[receiver_base].acked)
        {
            receiver_base++;
        }
    }

    printf("\nReceiver buffer (final):\n");
    for (int i = 0; i < TOTAL_PACKETS; i++)
    {
        printf("Seq No: %d, Acked: %s\n", receiver_buffer[i].seq_no, receiver_buffer[i].acked ? "true" : "false");
    }

    return 0;
}
