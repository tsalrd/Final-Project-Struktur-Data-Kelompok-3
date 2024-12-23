#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX_CAPACITY 5
#define MAX_SEATS 10

void clearscreen() {
    system(CLEAR);
}

typedef struct Queue {
    char name[50];
    struct Queue* next;
} Queue;

typedef struct Seat {
    char name[50];
} Seat;

typedef struct Stack {
    char movie[50];
    struct Stack* next;
} Stack;

typedef struct TicketNode {
    char name[50];
    int seatNumber;
    char movie[50];
    struct TicketNode* next;
} TicketNode;

void enqueue(Queue** front, Queue** rear, char* name, int* count) {
    if (*count >= MAX_CAPACITY) {
        printf("Antrian penuh! Tidak dapat menambah penonton.\n");
        return;
    }

    Queue* newPenonton = (Queue*)malloc(sizeof(Queue));
    strcpy(newPenonton->name, name);
    newPenonton->next = NULL;

    if (*rear == NULL) {
        *front = *rear = newPenonton;
    } else {
        (*rear)->next = newPenonton;
        *rear = newPenonton;
    }
    (*count)++;
    printf("%s telah ditambahkan ke antrian.\n", name);
}

char* dequeue(Queue** front, Queue** rear, int* count) {
    if (*front == NULL) {
        printf("Antrian kosong!\n");
        return NULL;
    }
    Queue* temp = *front;
    *front = (*front)->next;
    if (*front == NULL) *rear = NULL;

    char* name = (char*)malloc(50);
    strcpy(name, temp->name);
    free(temp);
    (*count)--;

    return name;
}

void resetQueue(Queue** front, Queue** rear, int* count) {
    while (*front != NULL) {
        Queue* temp = *front;
        *front = (*front)->next;
        free(temp);
    }
    *rear = NULL;
    *count = 0;
}

void removeByName(Queue** front, Queue** rear, char* name, int* count) {
    if (*front == NULL) {
        printf("Antrian kosong!\n");
        return;
    }

    Queue* temp = *front;
    Queue* prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            if (prev == NULL) {
                *front = temp->next;
            } else {
                prev->next = temp->next;
            }

            if (temp == *rear) {
                *rear = prev;
            }

            free(temp);
            (*count)--;
            printf("Penonton %s telah dihapus dari antrian.\n", name);
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Penonton %s tidak ditemukan dalam antrian.\n", name);
}

void push(Stack** top, char* movie) {
    Stack* newMovie = (Stack*)malloc(sizeof(Stack));
    strcpy(newMovie->movie, movie);
    newMovie->next = *top;
    *top = newMovie;
}

char* pop(Stack** top) {
    if (*top == NULL) {
        printf("Tidak ada film yang akan diputar!\n");
        return NULL;
    }

    Stack* temp = *top;
    char* movie = (char*)malloc(50);
    strcpy(movie, temp->movie);
    *top = (*top)->next;
    free(temp);

    return movie;
}

void displayStack(Stack* top) {
    if (top == NULL) {
        printf("Tidak ada film yang akan diputar!\n");
        return;
    }
    Stack* temp = top;
    while (temp != NULL) {
        printf("%s\n", temp->movie);
        temp = temp->next;
    }
}

void addTicket(TicketNode** head, char* name, int seatNumber, char* movie) {
    TicketNode* newTicket = (TicketNode*)malloc(sizeof(TicketNode));
    strcpy(newTicket->name, name);
    newTicket->seatNumber = seatNumber;
    strcpy(newTicket->movie, movie);
    newTicket->next = *head;
    *head = newTicket;
}

void displayTickets(TicketNode* head) {
    if (head == NULL) {
        printf("Riwayat transaksi tiket kosong!\n");
        return;
    }

    TicketNode* temp = head;
    while (temp != NULL) {
        printf("%s - Kursi %d - Film: %s\n", temp->name, temp->seatNumber, temp->movie);
        temp = temp->next;
    }
}

void initializeSeats(Seat seats[]) {
    for (int i = 0; i < MAX_SEATS; i++) {
        strcpy(seats[i].name, "Belum Terisi");
    }
}

void displaySeats(Seat seats[]) {
    for (int i = 0; i < MAX_SEATS; i++) {
        printf("%d. %s\n", i + 1, seats[i].name);
    }
}

int assignSeat(Seat seats[], char* name) {
    int seatNumber;
    while (1) {
        printf("Pilih kursi yang ingin dipesan. Kursi yang tersedia: \n");
        for (int i = 0; i < MAX_SEATS; i++) {
            if (strcmp(seats[i].name, "Belum Terisi") == 0) {
                printf("%d ", i + 1);
            }
        }
        printf("\nPilihan Anda: ");
        scanf("%d", &seatNumber);

        if (seatNumber < 1 || seatNumber > MAX_SEATS || strcmp(seats[seatNumber - 1].name, "Belum Terisi") != 0) {
            printf("Maaf, kursi yang Anda pesan sedang dipakai.\n");
            continue;
        }

        strcpy(seats[seatNumber - 1].name, name);
        printf("%s silahkan menduduki kursi %d.\n", name, seatNumber);
        return seatNumber;
    }
}

void removeSeat(Seat seats[]) {
    int seatNumber;
    printf("Pilih kursi yang ingin dihapus: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > MAX_SEATS || strcmp(seats[seatNumber - 1].name, "Belum Terisi") == 0) {
        printf("Kursi tidak valid atau sudah kosong!\n");
        return;
    }

    printf("%s telah keluar dari kursi %d.\n", seats[seatNumber - 1].name, seatNumber);
    strcpy(seats[seatNumber - 1].name, "Belum Terisi");
}

void displayQueue(Queue* front) {
    if (front == NULL) {
        printf("Antrian kosong!\n");
        return;
    }
    Queue* temp = front;
    printf("Antrian Penonton:\n");
    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
}

void saveHistoryToFile(TicketNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan riwayat!\n");
        return;
    }

    TicketNode* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s,%d,%s\n", temp->name, temp->seatNumber, temp->movie);
        temp = temp->next;
    }

    fclose(file);
    printf("Riwayat transaksi berhasil disimpan ke file %s.\n", filename);
}

void loadHistoryFromFile(TicketNode** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Gagal membuka file untuk membaca riwayat!\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50];
        int seatNumber;
        char movie[50];

        sscanf(line, "%[^,],%d,%[^\n]", name, &seatNumber, movie);
        addTicket(head, name, seatNumber, movie);
    }

    fclose(file);
    printf("Riwayat transaksi berhasil dimuat dari file %s.\n", filename);
}

void displayHistoryFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File riwayat transaksi tidak ditemukan!\n");
        return;
    }

    char line[100];
    printf("Riwayat Transaksi dari File:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int main() {
    Queue* front = NULL;
    Queue* rear = NULL;
    Stack* movieTop = NULL;
    TicketNode* ticketHead = NULL;
    Seat seats[MAX_SEATS];

    initializeSeats(seats);

    char name[50], movie[50];
    int choice, queueCount = 0;

    while (1) {
        clearscreen();
        printf("\n1. Tambah Film\n");
        printf("2. Tampilkan Film dalam Stack\n");
        printf("3. Putar Film\n");
        printf("4. Tambah Antrian\n");
        printf("5. Tampilkan Antrian\n");
        printf("6. Proses Antrian\n");
        printf("7. Tampilkan Kursi\n");
        printf("8. Hapus Penonton\n");
        printf("9. Tampilkan Riwayat\n");
        printf("10. Simpan Riwayat Ke File\n");
        printf("11. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Masukkan nama film: ");
                fgets(movie, sizeof(movie), stdin);
                movie[strcspn(movie, "\n")] = 0;
                push(&movieTop, movie);
                break;
            case 2:
                displayStack(movieTop);
                break;
            case 3:
                if (movieTop != NULL) {
                    char* currentMovie = pop(&movieTop);
                    printf("Memutar film: %s\n", currentMovie);

                    for (int i = 0; i < MAX_SEATS; i++) {
                        if (strcmp(seats[i].name, "Belum Terisi") != 0) {
                            addTicket(&ticketHead, seats[i].name, i + 1, currentMovie);
                            strcpy(seats[i].name, "Belum Terisi");
                        }
                    }
                }
                break;
            case 4:
                printf("Masukkan nama penonton: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                enqueue(&front, &rear, name, &queueCount);
                break;
            case 5:
                displayQueue(front);
                break;
            case 6:
                if (front != NULL) {
                    printf("\nProses Antrian:\n");

                    char* dequeuedName = dequeue(&front, &rear, &queueCount);
                    if (dequeuedName != NULL) {
                        int seatNumber = assignSeat(seats, dequeuedName);
                        printf("%s telah diproses dengan kursi nomor %d\n", dequeuedName, seatNumber);
                    }
                }
                getchar();
                break;
            case 7:
                displaySeats(seats);
                break;
            case 8:
                removeSeat(seats);
                break;
            case 9:
                displayTickets(ticketHead);
                break;
            case 10:
                saveHistoryToFile(ticketHead, "riwayat_transaksi.txt");
                break;
            case 11:
                resetQueue(&front, &rear, &queueCount);
                printf("Keluar dari program.\n");
                exit(0);
            default:
                printf("Pilihan tidak valid!\n");
        }
        printf("Tekan enter untuk melanjutkan...");
        getchar();
    }

    return 0;
}
