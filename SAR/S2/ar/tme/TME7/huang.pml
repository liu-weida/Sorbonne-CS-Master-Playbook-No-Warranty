#define N 3

mtype = {actif, inactif, ack, agr, mes, term};

chan canaux[N] = [N] of {mtype, int, byte};
int unack[N] = 0;
bool termine = false; 
byte last = 255;
int hfin; 
int nbagr[N] = 0; 

inline desactivation(id) {
    etat = inactif;
    if ::(unack[id] == 0)-> h++; last = id; hfin = h; nbagr[id] = 0; broadcast(id);
    fi;
}

inline broadcast(id) {
    byte i = 0;
    do
        :: ((i < N)&&i != id) ->
            canaux[i] ! term, h, id;
            i++;
        :: (i ==id ) -> i++;
        :: else ->
            break;
    od
}

proctype un_site(byte id) {
    int h = 0;
    mtype etat = actif;
    unack[id] = 0;
    int hrec;
    int emetteur;
    byte dest = (id + 1) % N;
    do
        :: (empty(canaux[id]) && (etat == actif)) ->
            if    
                :: (1) -> desactivation(id)
                :: (1) ->  
                   dest = (id + 1) % N;
                   etat = inactif;
                   canaux[dest] ! mes, h, id;
                   unack[id]++;
                   h = h + 1; 
                
            fi
        :: nempty(canaux[id]) ->
            if
                :: canaux[id]?mes, hrec, emetteur ->
    			if ::(h < hrec) -> h = hrec;
    				::else ->skip;
                            fi;
    			canaux[emetteur] ! ack, h, id;

                :: canaux[id]?ack, hrec, emetteur ->
                    unack[id] --;
                    if ::(h < hrec) -> h = hrec;
                    ::else -> skip;
                    fi;
                    etat = actif;
                :: canaux[id]?term, hrec, emetteur ->
                    if
                        :: (unack[id] > 0) ->
                            if ::(h < hrec) -> h = hrec;
                            ::else ->skip;
                            fi;
                        :: ((etat == inactif) && (unack[id] == 0)) -> 
                                h = hrec; hfin = hrec; last = emetteur;
                                canaux[emetteur] ! agr, h, id;
                        :: else -> skip;
                    fi
                :: canaux[id]?agr, hrec, _ ->
                    if
                    	::(hrec == hfin)->
                        	nbagr[id] = nbagr[id] + 1;
                        	if :: (nbagr[id] == N - 1) -> 
                        		last = id;
                            		termine = true; 
                            		break; 
                            	   ::else ->skip;
                        	fi;
                        ::else -> skip;
                    fi;
            fi;
    od;
}

init {
    byte i = 0;
    do
        :: (i < N) ->
            run un_site(i);
            i++;
        :: else ->
            break;
    od
}

