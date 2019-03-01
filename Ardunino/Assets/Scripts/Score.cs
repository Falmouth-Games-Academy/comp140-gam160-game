using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
//This is telling the project that I am using the information from a UI system


public class Score : MonoBehaviour
{
    public Arduino ArduinoS;
    //This opens the information in the inspector, which allows me to drag the Arduino script on


    private int score1 = 0;
    private int score2 = 0;
    //This code says that the score for player 1 and 2 starts at 0, when playing a new game

    public Text Player1;
    public Text Player2;
    //This codes opens the information in the inspector, which allows me to drag the scores for the different player in the correct box


    private void OnCollisionEnter(Collision other)
    {
        if(other.gameObject.name == "RightWall")
        {
            ArduinoS.WriteToArduino("L");
            score2++;
            Player2.text = score2.ToString();

            resetBall();
        }
        //This code is saying that if the ball hits the right wall, then add 1 to the player 2 score. This then updates the scoring on the canvas. 
        //Then light up the LED Light whenever the player 2 scores
        //The ball is then reset

        if (other.gameObject.name == "LeftWall")
        {
            ArduinoS.WriteToArduino("L");
            score1++;
            Player1.text = score1.ToString();

            resetBall();
        }
        //This code is saying that if the ball hits the left wall, then add 1 to the player 1 score. This then updates the scoring on the canvas. 
        //Then light up the LED Light whenever the player 1 scores
        //The ball is then reset
    }

    private void resetBall()
    {
        StartCoroutine(WaitingStart());
    }
    //This code is saying when the ball restarts, play the functions

    IEnumerator WaitingStart()
    {
        GetComponent<Rigidbody>().velocity = Vector3.zero;
        yield return new WaitForSeconds(3);
        transform.position = new Vector3(-1.3f, 5.25f, 0f);
        GetComponent<Ball>().ResetSpeed();
    }
    //This code is saying wait 3 seconds, then restart the ball at the starting position. Then play the reset speed function, which randomises the direction that the ball is travelling 

}
