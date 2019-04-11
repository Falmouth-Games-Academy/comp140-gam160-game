using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {

    public Transform target;
    //This script opens a target in the inspector, so I can the drag the player in to the target. 

    public Vector3 offset = new Vector3(0, 0, -10);

    public bool followX = true;

    public bool followY = true;


	void Start () {
        if (target == null)
            enabled = false;
	}
	
	void LateUpdate () {
        Vector3 targetPosition = target.position;

        if (!followX)
            targetPosition.x = transform.position.x;

        if (!followY)
            targetPosition.y = transform.position.y;

        transform.position = targetPosition + offset;
		
	}
    //All this script follows the player, when the player is moving in the game.
}
//This is from Jamie's power point
