using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;

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


    void Start()
    {
        characterController = GetComponent<CharacterController>();

    }

    void Update()
    {
        xdd.y -= gravity * Time.deltaTime;

        characterController.Move(xdd * Time.deltaTime);

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