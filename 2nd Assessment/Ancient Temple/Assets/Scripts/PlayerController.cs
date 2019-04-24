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

    Vector3 xdd = new Vector3(0,0,0);

    [SerializeField]
    protected float speed = 6.0f;
    //This is saying how fast the player can move when walking
    [SerializeField]
    protected float jumpSpeed = 10.0f;
    //This is saying how fast the player can jump
    [SerializeField]
    protected float gravity = 10.0f;
    //This is saying how high the player can jump


    private Vector3 moveDirection = Vector3.zero;

    void Start()
    {
        characterController = GetComponent<CharacterController>();

    }

    void Update()
    {
        // Apply gravity. Gravity is multiplied by deltaTime twice (once here, and once below
        // when the moveDirection is multiplied by deltaTime). This is because gravity should be applied
        // as an acceleration (ms^-2)
        xdd.y -= gravity * Time.deltaTime;

            // Move the controller
        characterController.Move(xdd * Time.deltaTime);

    }

    private void OnTriggerEnter(Collider other)
    { 


        if (other.tag == "Hazards")
            SceneManager.LoadScene("YouDied");
        //This code is saying if the player touches any objects that have the tag hazard, the player will die and the death scene will load.

    }
    public void MovePlayer(float x)
    {
        xdd.x = x;
    }
    public void Jump()
    {
       xdd.y = 10;
    }
}
//This code is from Gareth's power point