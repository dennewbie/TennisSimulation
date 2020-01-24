# Tennis Simulation

Suppose we have a player playing against a simulator. The simulator throws the ball that must be repelled by the player. The player (red rectangle) is placed on a regular map of the same size as in the picture. The player has a racket (black in the picture).


The simulator makes a point if the player fails to repel the ball, otherwise if the receiver hits the ball the point goes to the player.  The game proceeds in this way:

- The simulator throws the ball with a certain intensity that is identified by a random number from 1 to 10. 
- If the ball falls into the orange cell it is repelled by the batter who makes a point. 
- If the ball falls into a yellow cell the point goes to the hitter if the intensity of the shot is less than 3 otherwise the point goes to the simulator. 
- If the ball falls in a blue zone the point goes to the player. 
- The ball can never be thrown at the player.  

Simulate the match 100 times each consisting of 500 throws. The player wins if he rejects at least 80% of the throws. Print the winning statistics.
