#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_CARDS 20
#define CN_LENGTH 17
#define CVV_LENGTH 4
#define EXP_LENGTH 5
#define NUME_LENGTH 20

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

typedef struct
{
    char nume[NUME_LENGTH];
    char cn[CN_LENGTH];
    char exp[EXP_LENGTH];
    char cvv[CVV_LENGTH];
} Card;

void adaugare_card(Card cards[], int *nr_cards)
{
    clear_screen();
    if (*nr_cards >= MAX_CARDS)
    {
        printf(ANSI_COLOR_RED "You have reached the maximum number of cards.\n" ANSI_COLOR_RESET);
        return;
    }
    int ok = 0;

    printf("Cardholder: ");
    fgets(cards[*nr_cards].nume, sizeof(cards[*nr_cards].nume), stdin);
    // Remove trailing newline character if present
    if (cards[*nr_cards].nume[strlen(cards[*nr_cards].nume) - 1] == '\n')
        cards[*nr_cards].nume[strlen(cards[*nr_cards].nume) - 1] = '\0';

    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("Card number: ");
        scanf("%s", cards[*nr_cards].cn);
        getchar();

        if (strlen(cards[*nr_cards].cn) != 16)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].cn); i++)
            if (!isdigit(cards[*nr_cards].cn[i]))
            {
                ok = 0;
                break;
            }
        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    ok = 0;
    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("Expiration date: ");
        scanf("%s", cards[*nr_cards].exp);
        getchar();

        if (strlen(cards[*nr_cards].exp) != 4)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].exp); i++)
            if (!isdigit(cards[*nr_cards].exp[i]))
            {
                ok = 0;
                break;
            }

        char luna[3] = "";

        strncpy(luna, cards[*nr_cards].exp, 2);

        if (atoi(luna) > 12)
            ok = 0;

        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    ok = 0;
    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("CVV: ");
        scanf("%s", cards[*nr_cards].cvv);
        getchar();

        if (strlen(cards[*nr_cards].cvv) != 3)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].cvv); i++)
            if (!isdigit(cards[*nr_cards].cvv[i]))
            {
                ok = 0;
                break;
            }

        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    FILE *fp = fopen("cards.txt", "a");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error opening file.\n" ANSI_COLOR_RESET);
        return;
    }

    fprintf(fp, "%s\n%s\n%s\n%s\n", cards[*nr_cards].nume, cards[*nr_cards].cn, cards[*nr_cards].exp, cards[*nr_cards].cvv);
    fclose(fp);

    (*nr_cards)++;
    clear_screen();
}

void citire_fisier(Card cards[], int *nr_cards)
{
    FILE *fp = fopen("cards.txt", "r");
    if (fp == NULL)
    {
        printf("\033[0;31mError opening file.\n\033[0m"); // ANSI_COLOR_RED and ANSI_COLOR_RESET replaced by ANSI escape codes
        return;
    }

    char buffer[256];
    while ((*nr_cards < MAX_CARDS))
    {
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        strncpy(cards[*nr_cards].nume, buffer, NUME_LENGTH);

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        strncpy(cards[*nr_cards].cn, buffer, CN_LENGTH);

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        strncpy(cards[*nr_cards].exp, buffer, EXP_LENGTH);

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        strncpy(cards[*nr_cards].cvv, buffer, CVV_LENGTH);

        (*nr_cards)++;
    }

    fclose(fp);
}

void afisare_carduri(Card cards[], int *nr_cards, int ok)
{
    if (*nr_cards < 1)
    {
        clear_screen();
        printf("You haven't added any cards yet do you want to?");
        printf("\n1.Yes");
        printf("\n2.No/Return");
        printf("\n3.Exit");
        char o;
        scanf("%c", &o);
        getchar();
        clear_screen();
        switch (o)
        {
        case '1':
            adaugare_card(cards, nr_cards);
            return;
        case '2':
            return;
        case '3':
            printf("Exiting.....\n");
            exit(0);
        default:
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
        }
    }
    else
    {
        if (ok == 0)
        {
            printf("Here are your cards:\n\n");

            for (int i = 0; i < *nr_cards; i++)
            {
                printf("%i. ", i + 1);
                printf(ANSI_COLOR_CYAN "Cardholder Name: %s" ANSI_COLOR_RESET, cards[i].nume);
                printf(" - Card Number: %s", cards[i].cn);
                printf(" - ");
                printf(ANSI_COLOR_GREEN "Expiration Date: %s" ANSI_COLOR_RESET, cards[i].exp);
                printf(" - ");
                printf(ANSI_COLOR_YELLOW "CVV: %s" ANSI_COLOR_RESET, cards[i].cvv);
                printf("\n");
            }
            printf("\n1.Return.\n");
            printf("2.Exit.\n");
            printf("Enter your choice:");
            char choice2;
            scanf("%c", &choice2);
            getchar();

            if (choice2 == '1')
                return;
            else if (choice2 == '2')
            {
                clear_screen();
                printf("Exiting.....\n");
                sleep(1);
                exit(0);
            }
            else
            {
                clear_screen();
                printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
                sleep(3);
                clear_screen();
            }
        }
        else if (ok == 1)
        {
            while (1)
            {
                printf("Here are your cards:\n\n");

                for (int i = 0; i < *nr_cards; i++)
                {
                    printf("%i. ", i + 1);
                    printf(ANSI_COLOR_CYAN "Cardholder Name: %s" ANSI_COLOR_RESET, cards[i].nume);
                    printf(" - Card Number: %s\n", cards[i].cn);
                }
                printf("\n1.View card details.\n");
                printf("2.Return.\n");
                printf("3.Exit.\n");
                printf("Enter your choice:");
                char choice2;
                scanf(" %c", &choice2);
                getchar();
                if (choice2 == '1')
                {
                    printf("\nFor which card do you want to see the details?");
                    int choice3;
                    scanf("%d", &choice3);
                    getchar();

                    clear_screen();

                    if (choice3 < 1 || choice3 > *nr_cards)
                    {
                        clear_screen();
                        printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
                        sleep(3);
                    }
                    else
                    {
                        clear_screen();
                        printf(ANSI_COLOR_CYAN "Cardholder Name: %s" ANSI_COLOR_RESET, cards[choice3 - 1].nume);
                        printf(" - Card Number: %s", cards[choice3 - 1].cn);
                        printf(" - ");
                        printf(ANSI_COLOR_GREEN "Expiration Date: %s" ANSI_COLOR_RESET, cards[choice3 - 1].exp);
                        printf(" - ");
                        printf(ANSI_COLOR_YELLOW "CVV: %s" ANSI_COLOR_RESET, cards[choice3 - 1].cvv);
                        printf("\n");

                        printf("\n1.Return.");
                        printf("\n2.Exit.\n");
                        printf("Enter your choice:");
                        char choice4;
                        scanf(" %c", &choice4);
                        getchar();
                        if (choice4 == '1')
                        {
                            clear_screen();
                            continue;
                        }
                        else if (choice4 == '2')
                        {
                            clear_screen();
                            printf("Exiting.....\n");
                            sleep(1);
                            exit(0);
                        }
                        else
                        {
                            clear_screen();
                            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
                            sleep(3);
                        }
                    }
                }
                else if (choice2 == '2')
                    return;
                else if (choice2 == '3')
                {
                    clear_screen();
                    printf("Exiting.....\n");
                    sleep(1);
                    exit(0);
                }
                else
                {
                    clear_screen();
                    printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
                    sleep(3);
                }
            }
        }
    }
}

void edit_card(Card cards[], int *nr_cards)
{
    FILE *fp = fopen("cards.txt", "w");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error opening file.\n" ANSI_COLOR_RESET);
        return;
    }
    afisare_carduri(cards, nr_cards, 0);
    printf("\n0. Return\n");

    printf("\nWhich card do you wish to edit?\n");
    char editt_card;
    scanf("%c", &editt_card);
    getchar();
    if (!(isdigit(editt_card) && (atoi(&editt_card) - 1) < *nr_cards))
    {
        clear_screen();
        printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
        sleep(3);
        edit_card(cards, nr_cards);
    }
    if (atoi(&editt_card) == 0)
        return;
    clear_screen();
    printf("Cardholder: ");
    scanf("%s", cards[*nr_cards].nume);
    getchar();

    int ok = 0;

    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("Card number: ");
        scanf("%s", cards[*nr_cards].cn);
        getchar();

        if (strlen(cards[*nr_cards].cn) != 16)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].cn); i++)
            if (!isdigit(cards[*nr_cards].cn[i]))
            {
                ok = 0;
                break;
            }
        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    ok = 0;
    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("Expiration date: ");
        scanf("%s", cards[*nr_cards].exp);
        getchar();

        if (strlen(cards[*nr_cards].exp) != 4)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].exp); i++)
            if (!isdigit(cards[*nr_cards].exp[i]))
            {
                ok = 0;
                break;
            }
        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    ok = 0;
    while (!ok)
    {
        ok = 1;
        clear_screen();
        printf("CVV: ");
        scanf("%s", cards[*nr_cards].cvv);
        getchar();

        if (strlen(cards[*nr_cards].cvv) != 3)
            ok = 0;

        for (int i = 0; i < strlen(cards[*nr_cards].cvv); i++)
            if (!isdigit(cards[*nr_cards].cvv[i]))
            {
                ok = 0;
                break;
            }

        if (ok == 0)
        {
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    fclose(fp);

    clear_screen();
}

void remove_card(Card cards[], int *nr_cards)
{
    clear_screen();
    FILE *fp = fopen("cards.txt", "w");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error opening file.\n" ANSI_COLOR_RESET);
        return;
    }
    afisare_carduri(cards, nr_cards, 0);

    printf("\nWhich card do you wish to delete?\n");
    char del_card;
    scanf("%c", &del_card);
    getchar();
    if (!isdigit(del_card))
    {
        clear_screen();
        printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
        sleep(3);
        remove_card(cards, nr_cards);
    }

    for (int i = del_card - 1; i < *nr_cards - 1; i++)
        cards[i] = cards[i + 1];

    (*nr_cards)--;

    for (int i = 0; i < *nr_cards; i++)
        fprintf(fp, "%s\n%s\n%s\n%s\n", cards[*nr_cards].nume, cards[*nr_cards].cn, cards[*nr_cards].exp, cards[*nr_cards].cvv);
    fclose(fp);

    clear_screen();
}

int main()
{
    char choice;
    int nr_cards = 0;
    Card cards[MAX_CARDS];

    citire_fisier(cards, &nr_cards);

    while (1)
    {
        clear_screen();
        printf("Welcome to the Digital Wallet app.\n");
        printf("1. View existing cards.\n");
        printf("2. Add a new card.\n");
        printf("3. Remove an existing card.\n");
        printf("4. Edit card details\n");
        printf("5. Exit.\n");
        printf("Enter your choice: ");
        scanf("%c", &choice);
        getchar();
        clear_screen();

        switch (choice)
        {
        case '1':
            afisare_carduri(cards, &nr_cards, 1);
            break;
        case '2':
            adaugare_card(cards, &nr_cards);
            break;
        case '3':
            remove_card(cards, &nr_cards);
            break;
        case '4':
            edit_card(cards, &nr_cards);
            break;
        case '5':
            printf("Exiting.....\n");
            exit(0);
        default:
            clear_screen();
            printf(ANSI_COLOR_RED "Invalid input try again!\n" ANSI_COLOR_RESET);
            sleep(3);
        }
    }

    return 0;
}