using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ingrediant : MonoBehaviour {
    
    [SerializeField]
    private GameObject liquid;
    [SerializeField]
    private Transform spawnPoint;
    private bool isPouring;
    // Use this for initialization
    void Start () {
	}
	
	// Update is called once per frame
	void Update () {
        
        //When the player collised with the ingrediants collider it will spawn the ingrediant
        if (isPouring)
            pour();

    }

    //Create the ingrediant object and destroy after 2 seconds when called
    void pour()
    {
        //transform.eulerAngles = new Vector3(-90, transform.eulerAngles.y, transform.eulerAngles.z);
        GameObject drop = Instantiate(liquid, spawnPoint.position, Quaternion.identity);
        Destroy(drop, 2);
    }

    //when the shaker collides with the trigger ispouring is set to true
    private void OnTriggerEnter (Collider other)
    {
        if(other.transform.name.Contains("Shaker"))
            isPouring = true;
    }

    //when the shaker exits the trigger ispouring is set to false
    private void OnTriggerExit(Collider other)
    {
        if (other.transform.name.Contains("Shaker"))
            isPouring = false;
    }
}
