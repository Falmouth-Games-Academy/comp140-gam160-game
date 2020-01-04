using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball : MonoBehaviour
{
    public float speed = 10;
    //This sets the speed of the ball, which can be changed in the inspector

    
    private void Start()
    {
        ResetSpeed();
    }
    //This is saying as a game starts or restarts, call that function


    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.name == "Player1")
        {
            
            float y = hitFactor(transform.position,
                                col.transform.position,
                                col.collider.bounds.size.y);

            
            Vector3 dir = new Vector3(1, y).normalized;

            
            GetComponent<Rigidbody>().velocity = dir * speed;
        }
        //This code is saying if the ball hits the player 1 paddle, then turn around and go back towards the player 2 paddle
        //It is also saying, stay the same speed and don't get slower or faster.

       
        if (col.gameObject.name == "Player2")
        {
            
            float y = hitFactor(transform.position,
                                col.transform.position,
                                col.collider.bounds.size.y);

         
            Vector3 dir = new Vector3(-1, y).normalized;

            
            GetComponent<Rigidbody>().velocity = dir * speed;
        }
        //This code is saying if the ball hits the player 2 paddle, then turn around and go back towards the player 1 paddle
        //It is also saying, stay the same speed and don't get slower or faster.
    }

    float hitFactor(Vector3 ballPos, Vector3 racketPos, float racketHeight)
    {
        return (ballPos.y - racketPos.y) / racketHeight;
    }
    //

    public void ResetSpeed()
    {
        int dir = UnityEngine.Random.value >= 0.5f ? 1 : -1;
        GetComponent<Rigidbody>().velocity = ((Vector3.right * dir) + (Vector3.up * UnityEngine.Random.Range(-1.0f, 1.0f))) * speed;
        //This code makes sure that the ball goes into a random direction, left and right as well as at an angle every time that the ball restarts after a score
    }

}

