import numpy as np
import matplotlib.pyplot as plt 
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Slider 

# DEFINITION DES PARAMETRE 
g = 9.81          # gravity
L = 1.0           # Longueur  de la tige
m = 0.2           # Masse de la bille au bout de la tige  
M = 0.5           # Masse du chariot 
dt = 0.01

class PID() :
    def __init__(self, kp, ki , kd, limit):
        self.Kp = kp
        self.Ki = ki
        self.Kd = kd
        self.somme_err = 0.0
        self.err_precedent = 0.0
        self.limit = limit
        
    def update_params(self,tab):
        self.Kp = tab[0]
        self.Kd = tab[1]
        self.Ki = tab[2]
        
    def calculer(self, consigne, mesure, dt):
        error = consigne - mesure 
        
        P = self.Kp * error 
        
        self.somme_err = np.clip(self.somme_err + error * dt, -self.limit , self.limit)
        I = self.Ki * self.somme_err
        
        deriv= (error - self.err_precedent)/dt
        D = self.Kd * deriv
        
        self.err_precedent = error
        
        return P + I + D
    

class RobotPendule():
    def __init__(self, x_init , theta_init, m = 0.2, M = 0.5 , L= 1 ):
        self.m , self.M , self.L , self.g = m , M , L , 9.81
        
        self.x = x_init
        self.v = 0.0

        self.theta = theta_init
        self.omega = 0.0

    def update(self, Force_moteur , dt):
        
        sin_t, cos_t = np.sin(self.theta) , np.cos(self.theta)
                
        # Inertie equivalent du systeme
        denom = self.L * (self.M + self.m * sin_t**2)

        # Acceleration angulaire 
        alpha = (self.g * (self.M + self.m) * sin_t - (Force_moteur + self.m * L * self.omega**2 * sin_t) * cos_t)/denom
        
        # Acceleration Lineaire 
        a = (Force_moteur + self.m * self.L *(self.omega**2 * sin_t - alpha * cos_t))/(self.M + self.m)

        # Integration d'Euler pour mettre a jour la physique
        self.omega += alpha * dt
        self.theta += self.omega * dt 
        
        self.v += a * dt
        self.x += self.v * dt 
        
    def Reinitialiser(self, pid_in , pid_out):
        self.x = 0.0
        self.v = 0.0
        self.theta = 0.0
        pid_in.somme_err = 0
        pid_out.somme_err = 0
        pid_in.err_precedent = 0.0
        pid_out.err_precedent = 0.0 
        
        
        
# pid_exterieur = PID(0.8, 0.9, 1.0, 10)
# pid_interne = PID(-30.0, 0.0, -12.0, 10)

#Brendon
# pid_exterieur = PID(0.475, -0.01, 0.24, 10)
# pid_interne = PID(-39.75, 0.0, -13.3, 10)

pid_exterieur = PID(0.38, -0.005, 0.32, 10)

pid_interne = PID(-39.0, 0.0, -14.5, 10)

limit_moteur = 30

Robot = RobotPendule(x_init=0.0 , theta_init=0.1)
 
# Getion du graphique 


fig , ax = plt.subplots(figsize=(8 , 4))
plt.subplots_adjust(bottom=0.35) #Ajustement pour les slider

ax.set_xlim(-4.0 , 4.0)
ax.set_ylim(-0.2 , 2.0)
ax.axhline(0 , color='black', lw=1)

chariot, = ax.plot([], [] , marker='s' , markersize =5, label='Chariot(Point)')
Tige, = ax.plot([], [] , 'b-', lw=3 , label='Tige')
Text = ax.text(-1.9 , 1.3, '', fontsize=10, bbox=dict(facecolor='white', alpha=0.7))
ligne_cible, = ax.plot([] , [], 'b-', lw=1 , alpha=0.5 )

ax.legend()
ax.grid()

# Gestion des slider 

ax_slider1 = plt.axes([0.15, 0.20 , 0.25 ,0.03 ])
ax_slider2 = plt.axes([0.15, 0.12 , 0.25 ,0.03 ])
ax_slider4 = plt.axes([0.55, 0.20 , 0.25 ,0.03 ])
ax_slider6 = plt.axes([0.55, 0.12 , 0.25 ,0.03 ])
ax_slider3 = plt.axes([0.15, 0.05 , 0.25 ,0.03 ])
ax_slider5 = plt.axes([0.55, 0.05 , 0.25 ,0.03 ])

#  valinit=np.mean(limits)
Kp_angle = Slider(ax=ax_slider1 , label="Kp angle" , valmin= 0.0 ,valmax= 5.0 , valinit=pid_exterieur.Kp )
Kd_angle = Slider(ax=ax_slider2 , label="Kd angle" , valmin= 0.0 ,valmax= 3.0 , valinit=pid_exterieur.Kd )
Kp_pos = Slider(ax=ax_slider4 , label="Kp pos" , valmin= -50.0 ,valmax= 0.0 , valinit=pid_interne.Kp )
Kd_pos = Slider(ax=ax_slider6 , label="Kd pos" , valmin= -20 ,valmax= 0.0 , valinit=pid_interne.Kd )

Ki_angle = Slider(ax=ax_slider3 , label="Ki angle" , valmin= -0.9 ,valmax= 0.9 , valinit=pid_interne.Ki )
Ki_pos = Slider(ax=ax_slider5 , label="Ki pos" , valmin= -0.9 ,valmax= 0.9 , valinit=pid_interne.Ki )

Sliders = [Kp_angle, Kd_angle,Ki_angle, Kp_pos, Kd_pos, Ki_pos]


pas_bot = 0.1
x_target = Robot.x
x_target_interpolate = Robot.x
vitesse_consigne = 1.5 # Variable chargerde gerer la consigne 


def key_press(event):
    global x_target 
    
    if event.key == "right" :
        x_target += pas_bot
    elif event.key == "left" :
        x_target -= pas_bot
    
fig.canvas.mpl_connect("key_press_event", key_press)

def init():
    chariot.set_data([], [])
    Tige.set_data([], [])
    Text.set_text('')
    ligne_cible.set_data([], [])
    return chariot, Tige, ligne_cible, Text


def update(frame):
    global x_target , x_target_interpolate, vitesse_consigne, dt
    
    params = [0.0  for i in range(len(Sliders))]
    
    for k , slider in enumerate(Sliders):
        params[k] = slider.val
        
    ecart = x_target - x_target_interpolate
    pas_maximal = vitesse_consigne * dt
    
    if pas_maximal < abs(ecart) :
        x_target_interpolate += np.sign(ecart) * pas_maximal
    else :
        x_target_interpolate = x_target
        
    pid_exterieur.update_params(params[0:3]) 
    pid_interne.update_params(params[3:])
    
    consigne_angle = pid_exterieur.calculer(x_target_interpolate, Robot.x , dt)
    consigne_angle = np.clip(consigne_angle, np.radians(-10), np.radians(10))
    
    Force = pid_interne.calculer(consigne_angle , Robot.theta , dt )
    Force = np.clip(Force, -limit_moteur , limit_moteur)
    
    Robot.update(Force_moteur=Force ,dt=dt )
    
    chariot.set_data([Robot.x], [0.0])
    Tige.set_data([Robot.x , Robot.x + L * np.sin(Robot.theta)], [0.0 , L * np.cos(Robot.theta)])
    ligne_cible.set_data([x_target, x_target], [-1 , 1])
    
    Text.set_text(
        f"Controle : Utilisez les flèches GAUCHE / DROITE \n\n"
        f"Cible Reelle : {x_target:.2f} m\n "
        f"Cible Lisséé : {x_target_interpolate:.2f} m\n"
        f"Position du robot : {Robot.x} m\n"
        # f"Angle Actuel : {np.degrees(Robot.theta):.1f} m\n"
    )
    
    if Robot.x < -3 or 3 < Robot.x :
        Robot.Reinitialiser(pid_in=pid_interne , pid_out=pid_exterieur)
        
    
    return chariot, Tige , ligne_cible, Text

ani = FuncAnimation(fig, update, frames=200, init_func=init , interval=10 , blit=True)


plt.show()
ani.save("mobileRobot.gif", writer='pillow' , fps=50)



