1.//FILM/TITRE ,

2.//FILM[GENRE='Horreur']/TITRE ,

3.//FILM[TITRE = 'Alien']/RESUME ,

4.//FILM[ROLES/ROLE[NOM="Stewart" and PRENOM="James"]]/TITRE  ,

5.//FILM[ROLES/ROLE[NOM="Stewart" and PRENOM="James"] and ROLES/ROLE[NOM="Novak" and PRENOM="Kim"]]/TITRE ,

6.//FILM[RESUME]  ,

7.//FILM[not(RESUME)] ,

8.//FILM[TITRE = 'Vertigo']/MES/@idref ,

9.//FILM[TITRE = "Reservoir dogs"]/ROLES/ROLE[NOM="Keitel" and PRENOM="Harvey"]/INTITULE ,

10.//FILM[last()] ,

11.//FILM[TITRE="Shining"]/preceding-sibling::FILM[1]/TITRE  ,

12.//FILM[TITRE = 'Eyes Wide Shut']/MES/@idref ,

13.//FILM[contains(TITRE,"V")]/TITRE ,

14.//* [count(child::*) = 3]  ,

15.//*[contains(name(), 'TU')]  ,

'----------------------------------------------',

17. //ville[@nom = //restaurant[@etoile = 3][@ville = following-sibling[@etoile = 3]]/@ville] ,

18.//restaurant[menu[contains(nom,"salade") and @prix != following-sibling::menu[contains(nom,"salade")]/@prix]]/@nom

19.//menu[not(following::menu/@prix = @prix)]/@prix