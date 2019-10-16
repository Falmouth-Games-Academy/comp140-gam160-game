using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KeyboardControl : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        //Control the movement of the shaker with keyboard instead of controller
        transform.position = new Vector3(Input.GetAxis("Horizontal"), 0 ,Input.GetAxis("Vertical"));
	}
}
