using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;

// This script moves the character controller forward
// and sideways based on the arrow keys.
// It also jumps when pressing space.

public class PlayerController : MonoBehaviour
{
    CharacterController characterController;

    [SerializeField]
    protected float speed = 6.0f;
    //This is saying how fast the player can move when walking
    [SerializeField]
    protected float jumpSpeed = 10.0f;
    //This is saying how fast the player can jump
    [SerializeField]
    protected float gravity = 20.0f;
    //This is saying how high the player can jump


    private Vector3 moveDirection = Vector3.zero;

    void Start()
    {
        characterController = GetComponent<CharacterController>(); 
    }

    void Update()
    {
        if (characterController.isGrounded)
        {
            // We are grounded, so recalculate
            // move direction directly from axes

            moveDirection = new Vector3(Input.GetAxis("Horizontal"), 0.0f, 0.0f);
            moveDirection *= speed;
            //This is saying only move fowardwards and backwards
           

            if (Input.GetButtonDown("Jump"))
            {
                moveDirection.y = jumpSpeed;
            }
            //This is saying press space bar to jump
        }

            // Apply gravity. Gravity is multiplied by deltaTime twice (once here, and once below
            // when the moveDirection is multiplied by deltaTime). This is because gravity should be applied
            // as an acceleration (ms^-2)
            moveDirection.y -= gravity * Time.deltaTime;

            // Move the controller
            characterController.Move(moveDirection * Time.deltaTime);

    }

    private void OnTriggerEnter(Collider other)
    { 


        if (other.tag == "Hazards")
            SceneManager.LoadScene("YouDied");
        //This code is saying if the player touches any objects that have the tag hazard, the player will die and the death scene will load.

    }

}
//This code is from Gareth's power point