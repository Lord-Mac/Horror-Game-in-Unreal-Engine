'''
Implementare algoritm RRT  (rapidly-exploring random trees)

Gheorghe Marius Alecsandru 
Grupa 311

'''

'''
Cerinte:

1) Explicati algoritmul

2) Implementați-l/aplicați-l pentru o suprafață cu obstacole (generați/cautați
hărți de spații interioare).

'''

'''
==========================================
Rezolvare:
==========================================

=========================
1) Explicati algoritmul
=========================

--> Ce este RRT?

RRT este un algoritm de cautare randomizata conceput pentru a explora eficient spatii de stare multidimensionale.
Scopul sau este sa gaseasca o cale /o traiectorie valida de la un Punct de Start la un Punct Destinatie (Goal),
evitand in acelasi timp obstacolele din mediu.

--> Cum functioneaza?

Algoritmul RRT poate fi asociat cu radacinile unui copac care se extind cautand destinatia.
Aceste radacin cresc explorand aleator spatiul disponibil.

1.Initializarea: Algoritmul incepe cu un singur nod (radacina copacului), adica punctul de start.
2.Esantionarea (Sampling): La fiecare iteratie, algoritmul alege un punct complet aleatoriu pe harta (P_random).
3.Cautarea celui mai apropriat vecin: Cauta in copacul deja format nodul care este cel mai aproape de acest punct aleator (N_nearest).
4.Extinderera(Steering): Creeaza un nod nou (N_new) facand un pas (de distanta step_size) dinspre N_nearest catre P_random
5.Verificarea coliziunilor: se verifica daca linia dintre N_nearest si N_new loveste vreaun obstacol
    Daca loveste un obstacol, nodul este ignorat/aruncat
    Daca spatiul este liber, N_new este adaugat in copac, legat de N_nearest
6.Verificarea distantei: Daca N_new este suficient de aproape de Punctul de Destinatie, algoritmul se opreste, iar traiectoria 
este reconstruita mergand inapoi din tata in tata pana la Punctul de Start 
7.Repetare: Pasii 2-6 se repeta pana se gaseste Punctul de Destinatie sau pana se atinge numarul maxim de iteratii.   
'''
'''
=================================
2)Implementare algoritm
=================================
===========================================================   ALGORITM RRT   ======================================================================================================================
'''

##
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random
import math


class Node:
    """Clasa care reprezintă un nod în copacul RRT"""
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.parent = None
        # memoreaza coordonatele si parintele fiecarui nod,
        # parintele ne ajuta pentru a reconstruii inapoi drumul.

class RRT:
    def __init__(self, start, goal, obstacle_list, rand_area, step_size=0.5, goal_sample_rate=10, max_iter=2000):
        self.start = Node(start[0], start[1])
        self.goal = Node(goal[0], goal[1])
        self.min_rand, self.max_rand = rand_area  # limitele camerei (valorile maxime si minime pe care le poate lua x si y pentru a creea harta pe care ne aflam)
        self.step_size = step_size # marimea pasului luat 
        self.obstacle_list = obstacle_list # Lista de (x, y, latime, inaltime)
        self.goal_sample_rate = goal_sample_rate # este un procent care introduce o euristica in cautare , ca sa nu fie complet random , astfel cautarea este mai eficienta 
        self.max_iter = max_iter # nr maxim de iteratii
        self.node_list = [] # aici stocam ruta pe masura ce exploram

    def plan(self):
        #porneste dintr-un punct, in for arunca puncte random , verifica care e fizic cel mai aproape si inceraca sa se intinda spre el dc nu exista obstacole , pana cand este aproape la punctul destinatie
        """Functia principala care genereaza traiectoria"""
        self.node_list = [self.start]

        for i in range(self.max_iter):
            rnd_node = self.get_random_node()  # aleg un punct aleatoriu
            nearest_ind = self.get_nearest_node_index(self.node_list, rnd_node) # alege cel mai apropriat vecin_indice
            nearest_node = self.node_list[nearest_ind] #alege cel mai apropriat vecin
            
            new_node = self.steer(nearest_node, rnd_node) # facem pasul catrenodul ales mai sus 

            # Verificam daca segmentul dintre nearest_node si new_node loveste peretii--- adica daca drumul e sigur.
            if self.check_collision(nearest_node, new_node, self.obstacle_list):
                self.node_list.append(new_node) # daca e sigur il adaugam in lista

                # Verificam daca suntem suficient de aproape de destinatie
                if self.calc_dist_to_goal(new_node.x, new_node.y) <= self.step_size:
                    final_node = self.steer(new_node, self.goal) # daca nodul final este la o distanta <= decat pasul ne ducem catre el 
                    if self.check_collision(new_node, final_node, self.obstacle_list):
                        return self.generate_final_course(len(self.node_list) - 1) # daca nu exista colisiuni generam ruta catre punct

        return None

    def steer(self, from_node, to_node):
        new_node = Node(from_node.x, from_node.y) # copie a nodului in care ne aflam 
        d, theta = self.calc_distance_and_angle(from_node, to_node) # aflam distanta d si unghiul dintre asa ox si dreapta ce uneste punctele de inceput si final
        
        # Facem un pas controlat in directia punctului destinatie (calculam coordonatele pt x si y stiind directia in care ne indreptam)
        new_node.x += self.step_size * math.cos(theta)
        new_node.y += self.step_size * math.sin(theta)
        new_node.parent = from_node # salvam noile coordonate
        return new_node

    def get_random_node(self):
        if random.randint(0, 100) > self.goal_sample_rate: ## ideea euristicii , dc sample rate e mai mare de 10 din 100 de cazuri ==> prob ==90%
            rnd = Node(random.uniform(self.min_rand, self.max_rand),
                       random.uniform(self.min_rand, self.max_rand)) ## genereaza noi coordonate cu o repartitie uniforma
        else:
            rnd = Node(self.goal.x, self.goal.y)#in 10%% din cazuri directia de virare este catre nodul pe care il cautam 
        return rnd

    def get_nearest_node_index(self, node_list, rnd_node):# primim ca input lista de noduri pe care o avem actual si noul nod rendom ales
        dlist = [(node.x - rnd_node.x)**2 + (node.y - rnd_node.y)**2 for node in node_list] ## i afiecare nod pe rand din lista si calculeaza distanta intre el si nodul random
        return dlist.index(min(dlist)) ## returneaza index ul celui mai mic nod

    def check_collision(self, from_node, to_node, obstacle_list):
        """Verifică dacă linia dintre from_node și to_node intersectează un obstacol (dreptunghi)"""
        # Esantionam puncte de-a lungul segmentului pentru a nu rata peretii subtiri
        steps = int(self.step_size / 0.1) # Verificam la fiecare 0.1 unitati
        for i in range(steps + 1):
            t = i / max(steps, 1)
            x = from_node.x + t * (to_node.x - from_node.x)
            y = from_node.y + t * (to_node.y - from_node.y)
            
           
            for obs in obstacle_list:
                buffer = 0.2 ## distanta extra pe care o pastreaza fata de pereti pt a evita o coliziune
                if obs[0] == 'rect':## verifica ce forma au obstacolele
                    _, ox, oy, w, h = obs
                    if (ox - buffer <= x <= ox + w + buffer) and (oy - buffer <= y <= oy + h + buffer):
                        return False # A lovit un perete
                elif obs[0] == 'circle':## verifica ce forma au obstacolele
                    _, cx, cy, r = obs
                    # Distanta de la punct la centrul cercului
                    if math.hypot(x - cx, y - cy) <= r + buffer:
                        return False # A lovit un obstacol circular        
        return True # Traseu sigur

    def calc_distance_and_angle(self, from_node, to_node):
        dx = to_node.x - from_node.x #diferenta pe orizontala dintre puncte
        dy = to_node.y - from_node.y #diferenta pe verticala dintre puncte
        d = math.hypot(dx, dy) # calculeaza direct distanta in linie dreapta 
        theta = math.atan2(dy, dx) # nu da eroare la impartirea cu 0
        return d, theta

    def calc_dist_to_goal(self, x, y): # calculeaza distanta de la punctul actual la cel de destinatie
        return math.hypot(x - self.goal.x, y - self.goal.y)
        #ne ajuta cand ne apropriem de punctul de destinatie 

    def generate_final_course(self, goal_ind):
        path = [[self.goal.x, self.goal.y]]
        node = self.node_list[goal_ind]
        while node.parent is not None:
            path.append([node.x, node.y])
            node = node.parent
        path.append([self.start.x, self.start.y])
        return path
        #reconstruieste inapoi ruta mergand din parinte in parinte

# =========================================================
# Functia de vizualizare si testare (harta dreptunghiuri)
# =========================================================
def main(varianta,marime_pas=0.5,iteratie_maxima=3000):
    print("Începem planificarea RRT pentru spațiul interior...")
    
    # Parametrii hartii
    start = [1.0, 1.0]
    goal = [14.0, 13.0]
    
    # Harta unui apartament/cladiri
    # Format perete: (x_stanga_jos, y_stanga_jos, latime, inaltime)
    if(varianta==1):   
        obstacle_list = [
            # Peretii exteriori (Bordura de jos, sus, stanga, dreapta)
            ('rect',-1, -1, 17, 1),   # Jos
            ('rect',-1, 15, 17, 1),   # Sus
            ('rect',-1, 0, 1, 15),    # Stanga
            ('rect',15, 0, 1, 15),    # Dreapta
            
            # Compartimentare interioara (camere și coridoare)
            ('rect',4, 0, 1, 10),     # Perete vertical stanga
            ('rect',9, 5, 1, 10),     # Perete vertical dreapta
            ('rect',4, 5, 3, 1),      # Perete orizontal atasat la stanga (formeaza o camera)
            ('rect',10, 10, 3, 1),    # Perete orizontal dreapta sus
        ]
    if(varianta==2):
        obstacle_list = [
            # Peretii exteriori
            ('rect',-1, -1, 17, 1), ('rect',-1, 15, 17, 1), ('rect',-1, 0, 1, 15), ('rect',15, 0, 1, 15),
            
            # Holuri in zig-zag
            ('rect',0, 4, 11, 2),   # Perete orizontal jos (lasa loc în dreapta)
            ('rect',4, 9, 12, 2),   # Perete orizontal sus (lasa loc în stanga)
        ]

    if(varianta==3):
        obstacle_list = [
            # Peretii exteriori
            ('rect',-1, -1, 17, 1), ('rect',-1, 15, 17, 1), ('rect',-1, 0, 1, 15), ('rect',15, 0, 1, 15),
            
            # Randul de birouri de jos
            ('rect',3, 0, 1, 5), ('rect',8, 0, 1, 5), ('rect',13, 0, 1, 5),
            
            # Insula centrala
            ('rect',4, 7, 6, 2),
            
            # Randul de birouri de sus
            ('rect',3, 11, 1, 4), ('rect',8, 11, 1, 4),
        ]
    if(varianta==4):
        obstacle_list = [
            # Peretii exteriori
            ('rect',-1, -1, 17, 1), ('rect',-1, 15, 17, 1), ('rect',-1, 0, 1, 15), ('rect',15, 0, 1, 15),
            
            # Peretele despartitor cu o usa la mijloc
            ('rect',7, 0, 2, 6),    # Partea de jos a peretelui
            ('rect',7, 8, 2, 7),    # Partea de sus a peretelui (lasa o gaura intre y=6 și y=8)
        ]
    if(varianta==5):
        obstacle_list = [
        # Peretii exteriori (dreptunghiuri)
        ('rect', -1, -1, 17, 1),
        ('rect', -1, 15, 17, 1),
        ('rect', -1, 0, 1, 15),
        ('rect', 15, 0, 1, 15),
        
        # Cativa pereti despartitori
        ('rect', 0, 6, 5, 1),
        ('rect', 10, 6, 5, 1),
        
        # Obstacole rotunde (stalpi sau utilaje mari) - ('circle', x_centru, y_centru, raza)
        ('circle', 7.5, 3, 1.5),
        ('circle', 4, 10, 1.2),
        ('circle', 11, 11, 1.8),
        ('circle', 7.5, 8, 1.0)
    ]
    

    # Initializare RRT
    rrt = RRT(start=start, goal=goal, rand_area=[0, 15], obstacle_list=obstacle_list, step_size=marime_pas, max_iter=iteratie_maxima)
    path = rrt.plan()

    # Desenare grafic
    fig, ax = plt.subplots(figsize=(10, 10))

    
    # Desenam obstacolele in functie de tipul lor
    for obs in obstacle_list:
        if obs[0] == 'rect':
            _, ox, oy, w, h = obs
            rect = patches.Rectangle((ox, oy), w, h, linewidth=1, edgecolor='black', facecolor='gray')
            ax.add_patch(rect)
        elif obs[0] == 'circle':
            _, cx, cy, r = obs
            circle = patches.Circle((cx, cy), r, linewidth=1, edgecolor='black', facecolor='darkred')
            ax.add_patch(circle)

    # Desenam copacul explorat (liniile verzi/gri)
    for node in rrt.node_list:
        if node.parent:
            ax.plot([node.x, node.parent.x], [node.y, node.parent.y], "-g", alpha=0.3)
            
    # Desenam calea gasită (linia rosie)
    if path is not None:
        path = np.array(path)
        ax.plot(path[:, 0], path[:, 1], '-r', linewidth=2.5, label="Traiectorie validă")
    else:
        print("Nu a putut găsi o cale validă. Poți încerca să rulezi din nou (RRT are o componentă aleatorie).")

    ax.plot(start[0], start[1], "xb", markersize=10, label="Start")
    ax.plot(goal[0], goal[1], "xr", markersize=10, label="Destinație")
    
    plt.title("Planificare traiectorie RRT într-un spațiu interior (Indoor Map)")
    plt.xlim(0, 15)
    plt.ylim(0, 15)
    plt.legend()
    plt.grid(True)
    plt.show()




if __name__ == '__main__':
    main(1,0.5,10000)
    '''
    putem alege din 5 harti modificand primul parametru al functiei main . Harti= {1,2,3,4,5}
    al doilea parametru controleaza lungimea pasului 
    al treilea parametru modifica numarul maxim de iteratii
    '''

