#include "Plateau.h"

Plateau::Plateau(){
	int turn = 0;
	int passe = 0;
	noirs = 0;
	blancs = 0;
	for(int i = 0; i<8; i++){
		for (int j = 0; j < 8; j++){	
			if((i == 3 && j == 3) ||(i == 4 && j == 4)){
				plateau[i][j] = 2;
				noirs++;
			}
			else if ((i == 4 && j == 3) || (i == 3 && j == 4)){
				plateau[i][j] = 1;
				blancs++;
			}
			else
				plateau[i][j] = 0;
		}
	}
}

Plateau::~Plateau(){}
	

int Plateau::get_noirs(){
	return noirs;
}

int Plateau::get_blancs(){
	return blancs;
}

void Plateau::print_board(){
	cout << "  a b c d e f g h  " << endl;
    for(int i = 0; i < 8; i++){
        cout << i + 1;
        cout << " ";
	    for(int j = 0; j < 8; j++){
		    if(plateau [i][j] == 0)
		    	cout << ". ";
		    else if(plateau[i][j] == 1)
		    	cout << "X ";
		    else if(plateau[i][j] == 2)
		    	cout << "O ";
	    }
        cout << i+1 << endl;
    }
	cout << "  a b c d e f g h  " << endl;
}
/*construceur du plateau, le 1 c'est les blancs (X) et le 2 c'est les noirs (O)*/

bool Plateau::check_input(int x, int y){
    if(x > 8 || x < 0 || y > 8 || y < 0){
        cout << "Invalid entry" <<endl;
        return false;
    }
    else if(plateau[x][y] != 0){
        cout << "Invalid entry" <<endl;
        return false;
    }
    else
        return true;
}
/*check si il y a un pion de la meme couleur plus loin dans cette direction, le 2e check pour manger quoi*/

bool Plateau::check_direction(int x, int y, int direction[2]){
    int dx = direction[0];
    int dy = direction[1]; //donne la direction en X et en Y dans laquelle on cherche
    color = (turn + 1)%2 + 1; //pour savoir si on est au tour du joueur 1 ou 2 sans avoir modulo d'un nombre paire = 0 mais = 2
    
    for(int dist = 2; dist < 8; dist++){
		if(((x + dist*dx) > 7) || ((x + dist*dx) <0) || ((y + dist*dy) > 7) || ((y + dist*dy) <0) || (plateau[x + dist*dx][y + dist*dy] == 0)) {
			//check si on sort pas du plateau ou si on arrive pas sur une case vide
			return false;
		}
		else if (plateau[x + dist*dx][y + dist*dy] == color){ /*si effectivement il y a des pions a manger,
																on vient les mettre tous dans le vector d un coup */
			for (int i = 1; i < dist; i ++){
				pos_to_eat.push_back (x + i*dx);
				pos_to_eat.push_back (y + i*dy);
			}
			return true;
		}
	} 
	return false;
}

bool Plateau::check_eat(int x, int y){//corriger cette fct sinon il y aura une erreur dans les coins et sur les bords quand x + 1 <0 par ex
	color = (turn +1)%2 + 1;
	int count = 0;
	int direction[2];
	bool eat = false;
	//int direction[8][2]={0};    Devenu useless si on combien les fcts check_eat et check_direction
	for (int i = -1; i <=1; i++){
		if(x != 0 || i !=-1){ //Pour eviter qu'on ne sorte du plateau en regardant ce qu il y a derriere
			for (int j = -1; j <= 1; j++){//je sais que ca checke aussi la position meme du pion mais bon ca evite qlq lignes de codes pour une bete test
				if(y != 0 || j !=-1){
					if (plateau[x + i][y + j] != 0 && plateau[x + i][y + j] != color){
						/* alors la soit on bosse avec des vectors --> taille dimensionnable, comme pour checker qu'une direction apres
						* soit on bosse en tableau toujours avec d'office une taille de 8 fois 2 element pour avoir les 8 directions
						* avec des 0 quand il ne faut pas checker par la */
						direction [0]= i;
						direction [1]= j; //trouver un moyen d ecrire " direction = {i, j}; "
						if (check_direction (x, y, direction)){
							eat = true; //le if c'est pour que si une fois true reste true tout en continuant de cheker les autres directions
						}
						/*direction[count][0] = i;
						direction[count][1] = j;  //Devenu useless */
					}
				}
			}
		}
	}
	if (eat){
		pos_to_eat.push_back (x);
		pos_to_eat.push_back (y);
	}
	/*
	if (count == 0){
		direction[0][0] = 5; /*le marqueur en question, on pourrait eventuellement commencer a le deplacer pour faire lire des moities de tableau etc
	}  // devenu useless */
	return eat;
}/*maintenant encore faire une fonction qui goupille celle ci et check_direction en utilisant cette derniere 
 que si la rangée de la matrice direction est != {0,0}*/
 
void Plateau::eat(){
	color = (turn + 1)%2 + 1;
    for(int i =0; i < pos_to_eat.size() - 1;i += 2){
        plateau[pos_to_eat[i]][pos_to_eat[i + 1]] = color;
        if (color == 1){
			blancs++;
			if (i!=0){ //sinon va compter qu on mange un de trop chaque fois vu qu'ajoute aussi le pion qu'on a ajouté
				noirs--;
			}
		}
		else if (color == 2){
			noirs++;
			if (i!=0){
				blancs--;
			}
		}
    }
}

bool Plateau::check_notplay(){
/*
 * Vérifie si le joueur peut en effet passer son tour
 */
	bool r = true;
	for(int i=0; i<8; i++){
		for (int j = 0; j< 8; j++){
			if(plateau[i][j] == 0){
				if(check_eat(i, j)){
					cout << "A move is possible" << endl;
					r = false;
					return r;
				}
			}
		}
	}
	return r;
}

/*bool Plateau::end_of_game(){
	if (passe == 2)
		return true; //Aurait pu servir comme fonction de fin mais no need
	return false;
}*/
void Plateau::print_winner(){
	cout << "\n \n \nFinal plate \n \n \n" << endl;
	print_board();
	cout << "Number of black pawns (O) : " << noirs << endl;
	cout << "Number of white pawns (X) : " << blancs << endl;
	if(noirs < blancs)
		cout << "White player wins !" << endl;
	else if(blancs < noirs)
		cout << "Black player wins !" << endl;
	else
		cout << "It's a draw." << endl;
}

bool Plateau::game_over(){
	if (passe == 2)
		return true;
	else if (noirs + blancs == 64)
		return true;
	return false;
}


void Plateau::game(){
	while(!game_over()){
        print_board();
        player_turn();
    }
    print_winner();	
}
 
void Plateau::player_turn(){
    turn++;
    string input = "";
    color = (turn + 1)%2 + 1;
    int x;
    int y;
    
    if(color == 1){
        cout << "White player turn (X)" << endl;       
    }    
    
    else{
	    cout << "Black player turn (O)" << endl;       
    }
    
    while(true){
		cout << "Turn " << turn << endl;
		cout << "Number of black pawns (O) : " << noirs << endl;
		cout << "Number of white pawns (X) : " << blancs << endl;
		cout << "enter a position :";
		getline(cin, input);
		if(input.length() == 2){               
                y = input[0] - 'a';
                x = input[1] - '1';
                if(input[0]=='0' && input[1]=='0'){
					if(check_notplay()){			
						passe ++;
						cout << "The player skips he's turn" << endl;
						break;
					}
				}
                else if(check_input(x,y)){
					pos_to_eat.clear();					
                    if(check_eat(x,y)){
						passe = 0;
                        eat();
                        break;
					}
                    else
                        cout << "Impossible move" << endl;
				}
		}          
        else
            cout << "Invalid entry" << endl;
	}
}
