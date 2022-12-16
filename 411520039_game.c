#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void game_begin();
void each_day_begin(int *dollars_ptr, int *speed_ptr, int *price_ptr, int *booster_limit_ptr);

int main(){

    int dollars = 100, speed = 15, price = 30;
    int *dollars_ptr = &dollars, *speed_ptr = &speed, *price_ptr = &price;
    int revenue = price * 180 / speed;
    //0 : speed booster, 1 : price booster, 2 : area booster
    int booster_num[3] = {5, 5, 5};
    int speed_upgrade_fee = 50, flavor_upgrade_fee = 100;
    int check_area;
    int flag = 1, go_on = 1;
    int amountHotdog;

    int lotterySize = 1;
    int lotteryShow[100][100] = {0};
    int lotteryDigit[100][100] = {0};
    int lotteryReal[100][100] = {0};

    //while player does't choose to leave the game
    while(go_on){
        srand(time(NULL));

        //to define the value of booster limitation
        //0 : speed booster, 1 : price booster, 2 : area booster
        int booster_limit[3] = {random() % 11 + 5, random() % 11 + 5, random() % 11 + 5};
        int *booster_limit_ptr = &booster_limit[0];

        //game begin
        game_begin();

        //initialize the value of each day begin
        int earned_money = 0;
        int area_result[10] = {0};
        int lotteryNum, lotteryFree = 0, lotteryCost = 500;
        int lotteryRemain = 0;
        int tmpPrizeType;
        int tmpMaxDigit, tmpNowDigit;
        int tmpMaxDigitContent, tmpNowDigitContent;
        int selectRow, selectColumn;
        
        each_day_begin(&dollars, &speed, &price, &booster_limit[0]);
        //when each day begin
        //printf("Chop chop, It's dawn.\n");
        //printf("You have %d dollars.\n", dollars);
        //printf("You need %d minutes to make a hotdog.\n", speed);
        //printf("The price of a hotdog is $%d.\n", price);
        //printf("The slots for each booster is %d speed booster(s), %d price booster(s), %d area booster(s).\n", booster_limit[0], booster_limit[1], booster_limit[2]);
        
        //to limit the numbers of booster holding in hand
        for(int i = 0; i < 3; i++){
            if(booster_num[i] > booster_limit[i])
                booster_num[i] = booster_limit[i];
        }
        printf("You have %d speed booster(s), %d price booster(s), %d area booster(s).\n", booster_num[0], booster_num[1], booster_num[2]);
        
        //boosters choosing section(open or close)
        //0: speed booster, 1: price booster, 2: area booster
        int booster_switch[3] = {0}, booster_choice = 0;
        while(booster_choice != 4){
            printf("Open/Close boosters:\n");
            printf("  [1] Speed booster (now %s)\n", booster_switch[0] ? "open" : "close");
            printf("  [2] Price booster (now %s)\n", booster_switch[1] ? "open" : "close");
            printf("  [3] Area  booster (now %s)\n", booster_switch[2] ? "open" : "close");
            printf("  [4] Finish\n");
            printf("Enter the number(s): ");
            scanf("%d", &booster_choice);

            if(booster_choice < 1 || booster_choice > 4)
                printf("Invalid input!!!!\n");
            if(booster_choice == 1)
                booster_switch[0] = !booster_switch[0];
            else if(booster_choice == 2)
                booster_switch[1] = !booster_switch[1];
            else if(booster_choice == 3)
                booster_switch[2] = !booster_switch[2];
        }

        //to determine whether the quantity is available to open(1 open : close)
        for(int i = 0; i < 2; i++){
            if(booster_num[i] == 0)
                booster_switch[i] = 0;
            if(booster_switch[i] == 1)
                booster_num[i]--;
        }

        //choose action section
        int action[5] = {0};
        action[4] = 1;
        printf("Actions you can take for each area:\n");
        printf("  [1] Sell the hotdogs\n");
        printf("  [2] Improve your cooking speed\n");
        printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n", speed_upgrade_fee, speed_upgrade_fee * 2, speed_upgrade_fee * 4, speed_upgrade_fee * 8);
        printf("  [3] Improve your hotdog flavor\n");
        printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n", flavor_upgrade_fee, flavor_upgrade_fee * 2, flavor_upgrade_fee * 4, flavor_upgrade_fee * 8);
        printf("Enter the number(s): ");
        for(int i = 0; i < 4; i++){
            scanf("%d", &action[i]);
        }

        //rechoose the invalid action
        int flag = 0;
        while(flag != 4){
            if(action[flag] < 1 || action[flag] > 3){
                printf("Invalid input!!!!\n");
                printf("Actions you can take at Area %d:\n", flag + 1);
                printf("  [1] Sell the hotdogs\n");
                printf("  [2] Improve your cooking speed\n");
                printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n", speed_upgrade_fee, speed_upgrade_fee * 2, speed_upgrade_fee * 4, speed_upgrade_fee * 8);
                printf("  [3] Improve your hotdog flavor\n");
                printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n", flavor_upgrade_fee, flavor_upgrade_fee * 2, flavor_upgrade_fee * 4, flavor_upgrade_fee * 8);
                printf("Enter the number(s): ");
                scanf("%d", &action[flag]);
            }else{
                flag++; 
            }
        }

        //to determine whether the actions in each area is valid
        earned_money = 0;
        for(int area = 0; area < 4 + booster_switch[2]; area++){
            amountHotdog = 180 / speed * (1 + booster_switch[0]);
            
            if((action[area] == 2 && dollars < speed_upgrade_fee) || 
               (action[area] == 3 && dollars < flavor_upgrade_fee)){
                action[area] = 4; 
            }else if(action[area] == 2 && speed == 1){
                action[area] = 5;
            }
            
            if(action[area] == 1 || action[area] == 4 || action[area] == 5){
                area_result[2 * area] = amountHotdog;
                area_result[2 * area + 1] = amountHotdog * price * (1 + booster_switch[1]);
                earned_money += amountHotdog * price * (1 + booster_switch[1]);
                dollars += amountHotdog * price * (1 + booster_switch[1]);
            }else if(action[area] == 2){
                speed--;
                dollars -= speed_upgrade_fee;
                speed_upgrade_fee *= 2;
            }else if(action[area] == 3){
                price += 10;
                dollars -= flavor_upgrade_fee;
                flavor_upgrade_fee *= 2;
            }
        }
        
        //summed up earned_money
        printf("Well done, you earn $%d today.\n", earned_money);

        //checking section
        check_area = 0;
        while(check_area != 4 + booster_switch[2]){
            printf("Which result of the area you want to check?\n");
            for(int i = 1; i <= 4 + booster_switch[2]; i++){
                printf("  [%d] Area %d\n", i, i);
            }
            printf("  [%d] Done\n", 5 + booster_switch[2]);
            printf("Enter the number(s): ");
            scanf("%d", &check_area);
            if(check_area < 1 || check_area > 5 + booster_switch[2]){
                printf("Invalid input!!!!\n");
                continue;
            }
            if(check_area == 5 + booster_switch[2]){
                break;
            }

            check_area -= 1;
            if(action[check_area] == 1){
                printf("You make %d hotdogs here!\n", area_result[2 * check_area]);
                printf("You earn $%d!\n", area_result[2 * check_area + 1]);
            }else if(action[check_area] == 2){
                printf("You glimpse the secret of wind.\n");
                printf("Your hands can move faster now.\n");
            }else if(action[check_area] == 3){
                printf("You feel the soul of the ingredients.\n");
                printf("Your hotdogs are more appetizing now.\n");
            }else if(action[check_area] == 4){
                printf("Can't you tell how poor you are?\n");
				printf("Go vending your hotdogs instead of thinking about self-improvement!\n");
                printf("You make %d hotdogs here!\n", area_result[2 * check_area]);
                printf("You earn $%d!\n", area_result[2 * check_area + 1]);
            }else if(action[check_area] <= 5){
                printf("Do you want to travel through time?\n");
                printf("GO WORK!!\n");
                printf("You make %d hotdogs here!\n", area_result[2 * check_area]);
                printf("You earn $%d!\n", area_result[2 * check_area + 1]);
            }
        }

        //continue or exit
        go_on = 0;
        while(go_on != 1 && go_on != 2){
            printf("Do you want to continue or exit?\n");
            printf("  [1] Continue\n");
            printf("  [2] Exit\n");
            printf("Enter the number(s): ");
            scanf("%d", &go_on);
            if(go_on != 1 && go_on != 2)
                printf("Invalid input!!!\n");
        }
        if(go_on == 2)
            break;

        //lottery part
        printf("You get one free choice.\n");
        lotteryFree++;
        flag = 1;

        while(flag){
            if(lotteryRemain == 0){
                lotteryCost = 500;
                lotterySize += 2;
                lotteryRemain = lotterySize * lotterySize;
                for(int i = 0; i < lotterySize; i++){
                    for(int j = 0; j < lotterySize; j++){
                        lotteryShow[i][j] = i * lotterySize + j + 1;
                        tmpNowDigit = lotteryShow[i][j];
                        tmpPrizeType = 0;
                        tmpNowDigitContent = 0;
                        while(tmpNowDigit){
                            tmpPrizeType = tmpPrizeType * 16 + tmpNowDigit % 10;
                            tmpNowDigit /= 10;
                            tmpNowDigitContent++;
                        }
                        lotteryReal[i][j] = tmpPrizeType % 9 + 1;
                        lotteryDigit[i][j] = tmpNowDigitContent;
                    }
                }
            }

            tmpMaxDigit = lotterySize * lotterySize;
            tmpMaxDigitContent = 0;
            while(tmpMaxDigit >= 1){
                tmpMaxDigit /= 10;
                tmpMaxDigitContent++;
            }

            //printf the lottery table
            for(int i = 0; i < lotterySize; i++){
                printf("+");
                for(int j = 0; j < lotterySize; j++){
                    for (int k = 0; k < tmpMaxDigitContent + 2; k++){
                        printf("-");
                    }
                    printf("+");
                }
                printf("\n|");
                for(int j = 0; j < lotterySize; j++){
                    if(lotteryReal[i][j] == -1){
                        for(int k = 0; k < tmpMaxDigitContent - 1; k++){
                            printf(" ");
                        }
                        printf(" x |");
                    }else{
                        for(int k = 0; k < tmpMaxDigitContent - lotteryDigit[i][j]; k++){
                            printf(" ");
                        }
                        printf(" %d |", lotteryShow[i][j]);
                    }
                }
                printf("\n");
            }
            printf("+");
            for(int j = 0; j < lotterySize; j++){
                for (int k = 0; k < tmpMaxDigitContent + 2; k++){
                    printf("-");
                }
                printf("+");
            }
            printf("\n");

            //choosing lottery
            printf("You can choose\n");
            printf("  [number on cell] to open (- $%d)\n", lotteryFree == 0 ? lotteryCost : 0);
            printf("  [0] to continue the game\n");
            printf("Enter the number(s): ");
            scanf("%d", &lotteryNum);
            selectRow = (lotteryNum - 1) / lotterySize;
            selectColumn = (lotteryNum - 1) % lotterySize;
            if(lotteryNum < 0 || lotteryNum > lotterySize * lotterySize || lotteryReal[selectRow][selectColumn] == -1){
                printf("Invalid input!!!!\n");
                continue;
            }
            if(lotteryNum == 0)
                break;

            //determine whether the money is available
            if(lotteryFree == 0 && lotteryCost > dollars){
                printf("You have no money!\n");
                break;
            }
            if(lotteryFree == 0){
                dollars -= lotteryCost;
                lotteryCost += 500;
            }else{
                lotteryFree--;
            }

            while(flag){
                srand(time(NULL));
                int prize_type = random() % 9 + 1;
                printf("%d\n", prize_type);
                lotteryReal[selectRow][selectColumn] = -1;
                lotteryRemain--;

                if(prize_type == 1){
                    printf("Fortune, fortune! You get $%d!\n", 100 * price);
                    dollars += 100 * price;
                }else if(prize_type == 2){
                    printf("You get an extra choice!\n");
                    lotteryFree++;
                }else if(prize_type >= 7){
                    printf("You get a booster!!\n");
                    if(prize_type == 7)
                        booster_num[0]++;
                    else if(prize_type == 8)
                        booster_num[1]++;
                    else if(prize_type == 9)
                        booster_num[2]++;
                }else if(prize_type >= 3 && prize_type <= 6){
                    if(prize_type == 3){
                        selectRow = (selectRow - 1 + lotterySize) % lotterySize;
                    }else if(prize_type == 4){
                        selectRow = (selectRow + 1) % prize_type;
                    }else if(prize_type == 5){
                        selectColumn = (selectColumn - 1 + lotterySize) % lotterySize;
                    }else if(prize_type == 6){
                        selectColumn = (selectColumn + 1) % lotterySize;
                    }
                    
                    if(lotteryReal[selectRow][selectColumn] == -1){
                        printf("bad Luck :(\n");
                        break;
                    }else{
                        printf("Another open on %d!\n", selectRow * lotterySize + selectColumn + 1);
                        flag = 1;
                        lotteryReal[selectRow][selectColumn] = -1;
                        lotteryRemain--;
                    }
                }
                break;
            }
        }
    }


    printf("We will miss you. Bye!\n");


    return 0;
}

void game_begin(){
    printf("Welcome, young boss!\n");
}

void each_day_begin(int *dollars_ptr, int *speed_ptr, int *price_ptr, int *booster_limit_ptr){
    printf("Chop chop, It's dawn.\n");
    printf("You have %d dollars.\n", *dollars_ptr);
    printf("You need %d minutes to make a hotdog.\n", *speed_ptr);
    printf("The price of a hotdog is $%d.\n", *price_ptr);
    printf("The slots for each booster is %d speed booster(s), %d price booster(s), %d area booster(s).\n", *booster_limit_ptr, *booster_limit_ptr + 1, *booster_limit_ptr + 2);
}



