using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Key : MonoBehaviour
{

    public GameObject targetObject;
    //This opens a target object into the inspector, so that I can then drag the object that I want the key to open


    void OnTriggerEnter(Collider other)
    {
        if (targetObject == null)
            return;

        if (other.tag != "Player")
            return;

        targetObject.SetActive(false);

        Destroy(gameObject);
        //This code is saying if the player toches the key, than destory the target game object and the key. 

    }
}
