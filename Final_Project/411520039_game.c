#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void background_introduction(int *go_on);

void game_begin();

void each_day_begin(int *dollars, int *speed, int *price);

void map_system(int *dollars, int *booster_num);

int main(){

    int dollars = 100, speed = 15, price = 30;
    int revenue = price * 180 / speed;

    //0 : speed booster, 1 : price booster, 2 : area booster
    int booster_num[3] = {0};
    int speed_upgrade_fee = 50, flavor_upgrade_fee = 100;
    int check_area;
    int flag = 1, go_on = 1;
    int amountHotdog;

    int lotterySize = 1;
    int lotteryShow[100][100] = {0};
    int lotteryDigit[100][100] = {0};
    int lotteryReal[100][100] = {0};
    int prize_type;

    //background intorduction
    background_introduction(&go_on);

    //set seed
    time_t t;
    srand(time(&t));

    //to define the value of booster limitation
    int booster_limit = rand() % 11 + 5;

    // to record the booster order(n: the order)
    int n = 0;
    int booster_record[booster_limit];

    if(go_on == 1){
        //game begin
        game_begin();

        printf("You have %d slots for your boosters.\n", booster_limit);
        printf("If your booster slots is filled.\nYou will lose the earlst booster you got!!!\n");
    }
    //while player does't choose to leave the game
    while(go_on){

        //initialize the value of each day begin
        int earned_money = 0;
        int area_result[10] = {0};
        int lotteryNum, lotteryFree = 0, lotteryCost = 500;
        int lotteryRemain = 0;
        int tmpPrizeType;
        int tmpMaxDigit, tmpNowDigit;
        int tmpMaxDigitContent, tmpNowDigitContent;
        int selectRow, selectColumn;
        int booster_total = 0;
        
        //when each day begin
        each_day_begin(&dollars, &speed, &price);
        
        //to limit the numbers of booster holding in hand
        for(int i = 0; i < 3; i++){
            booster_total += booster_num[i];
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

            map_system(&dollars, booster_num);
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

        //lottery section
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
            //set seed
            srand(time(NULL));
            int prize_type = rand() % 9 + 1;
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
            printf("You have %d dollars.\n", dollars);
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

            //determine which prize player will get
            while(flag){

                                    //test//
                             //printf("%d\n", prize_type);  
                //printf("%d %d\n", booster_total, booster_limit);

                lotteryReal[selectRow][selectColumn] = -1;
                lotteryRemain--;

                if(prize_type == 1){
                    printf("Fortune, fortune! You get $%d!\n", 100 * price);
                    dollars += 100 * price;
                }else if(prize_type == 2){
                    printf("You get an extra choice!\n");
                    lotteryFree++;
                }else if(prize_type >= 7 && booster_total < booster_limit){
                    //if holding quantity of boosters do not exceed the limitation
                    if(prize_type == 7){
                        printf("You get a speed booster!!\n");
                        booster_total++;
                        booster_num[0]++;
                        booster_record[n] = 1;
                        n++;
                    }else if(prize_type == 8){
                        printf("You get a price booster!!\n");
                        booster_total++;
                        booster_num[1]++;
                        booster_record[n] = 2;
                        n++;
                    }
                    else if(prize_type == 9){
                        printf("You get an area booster!!\n");
                        booster_total++;
                        booster_num[2]++;
                        booster_record[n] = 3;
                        n++;
                    }
                }else if(prize_type >= 7 && booster_total == booster_limit){
                    n = 0;
                    //if holding quantity of boosters do not exceed the limitation
                    printf("You have no slots!!!!\n");
                    printf("The earliest booster you got will be replaced.\n");
                    if(prize_type == 7){
                        booster_num[0]++;
                        printf("You get a speed booster.\n");
                    }else if(prize_type == 8){
                        booster_num[1]++;
                        printf("You get a price booster.\n");
                    }else if(prize_type == 9){
                        booster_num[2]++;
                        printf("You get an area booster.\n");
                    }
                    //remove the numbers of the earlist booster player got
                    if(booster_record[0] == 1){
                        booster_num[0]--;
                    }else if(booster_record[0] == 2){
                        booster_num[1]--;
                    }else if(booster_record[0] == 3){
                        booster_num[2]--;
                    }

                    //change the booster record order(remove first booster, left-shift all boosters order, and record new booster)
                    for(int i = 0; i < booster_limit - 1; i++){
                        booster_record[i] = booster_record[i + 1];
                        booster_record[booster_limit - 1] = prize_type - 6;
                    }
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
                        printf("Bad Luck :(\n");
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

    //end of the game
    End:
    printf("We will miss you. Bye!\n");


    return 0;
}



//function part

void background_introduction(int *go_on){
    int action;
    printf("Welcome to the food truck game!\n");
    printf("In this game, you're a hotdodg stall.\n");
    printf("Your dream is to become a professional ping-pong player.\n");
    printf("You need to earn as more as money you can.\n");
    printf("Press the number to continue:\n");
    printf("  [1] Yeah, I'm ready\n");
    printf("  [2] Maybe try it later\n");
    printf("Enter your action: ");
    scanf("%d", &action);
    if(action == 2)
        *go_on = 0;
}

void game_begin(){
    printf("Welcome, young boss!\n");
}

void each_day_begin(int *dollars, int *speed, int *price){
    printf("Chop chop, It's dawn.\n");
    printf("You have %d dollars.\n", *dollars);
    printf("You need %d minutes to make a hotdog.\n", *speed);
    printf("The price of a hotdog is $%d.\n", *price);
}

void map_system(int *dollars, int *booster_num){
    srand(time(NULL));
    int map[8][8];
    int playerRow = 0, playerCol = 0;
    int moneyRow = rand() % 7 + 1, moneyCol = rand() % 7 + 1;
    int boosterRow = rand() % 7 + 1, boosterCol = rand() % 7 + 1;
    int moving_fee = 25;
    
    //initialize all the array to 0
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            map[i][j] = 0;
        }
    }
    map[0][0] = 1;

    while(1){
        int action;
        map[moneyRow][moneyCol] = 2;
        map[boosterRow][boosterCol] = 3;
        map[playerRow][playerCol] = 1;
        
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(map[i][j] == 1)
                    printf("P ");
                else if(map[i][j] == 2)
                    printf("M ");
                else if(map[i][j] == 3)
                    printf("B ");
                else if(map[i][j] == 0)
                    printf(". ");
            }
            printf("\n");
        }

        //direction choosing
        printf("You have %d dollars.\n", *dollars);
        printf("You can choose:\n");
        printf("  [1] Up    (- $%d)\n", moving_fee);
        printf("  [2] Down  (- $%d)\n", moving_fee);
        printf("  [3] Left  (- $%d)\n", moving_fee);
        printf("  [4] Right (- $%d)\n", moving_fee);
        printf("  [5] Exit\n");
        printf("Enter the action: ");
        scanf("%d", &action);

        if(action >= 1 && action <= 4 && *dollars >= moving_fee){
            *dollars -= moving_fee;
        }
        //determine whether player has available fee
        if(*dollars < moving_fee){
            printf("You have no money!!!\n");
            break;
        }

        //moving...
        if(action <= 4 && action >= 1 && ((playerRow > 0 || playerRow < 8) && (playerCol > 0 || playerCol < 8))){
            map[playerRow][playerCol] = 0;
            if(action == 1 && playerRow > 0){
                playerRow -= 1;
            }else if(action == 2 && playerRow < 7){
                playerRow += 1;
            }else if(action == 3 && playerCol > 0){
                playerCol -= 1;
            }else if(action == 4 && playerCol < 7){
                playerCol += 1;
            }
            map[playerRow][playerCol] = 1;
        }else if(action == 5){
            break;
        }else{
            printf("Invalid input!!!!\n");
        }

        //if player reach money icon, win the money and create a new money icon.
        if(playerRow == moneyRow && playerCol == moneyCol){
            printf("Fortune, fortune! You get %d!\n", (playerRow + 1) * (playerCol + 1) * 10);
            *dollars += (playerRow + 1) * (playerCol + 1) * 10;
            map[moneyRow][moneyCol] = 0;
            moneyRow = rand() % 8;
            moneyCol = rand() % 8;
            map[moneyRow][moneyCol] = 1;
        }

        //if player reach booster icon, get the booster and create a new booster icon
        if(playerRow == boosterRow && playerCol == boosterCol){
            printf("You got a random booster!!!\n");
            booster_num[rand() % 3]++;
            map[boosterRow][boosterCol] = 0;
            boosterRow = rand() % 8;
            boosterCol = rand() % 8;
            map[boosterRow][boosterCol] = 2;
        }
    }
}
