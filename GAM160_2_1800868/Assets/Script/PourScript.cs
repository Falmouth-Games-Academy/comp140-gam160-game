using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PourScript : MonoBehaviour {

    [SerializeField]
    private Transform spawnPoint;
    [SerializeField]
    private GameObject drink;
    [SerializeField]
    private Shakermeter sm;
    [SerializeField]
    private Arduino arduino;


    // Use this for initialization
    void Start () {
        
	}
	
	// Update is called once per frame
	void Update () {

        //Debug.Log("Lid is open: " + lidIsOpen);

        Debug.Log("is shaken: " + sm.isShaken);

        if (sm != null )
        {
            if (sm.isShaken)
            {
                if (transform.eulerAngles.z < 220 && transform.eulerAngles.z > 150)
                {
                    Instantiate(drink, spawnPoint.position, Quaternion.identity);
                }
            }
            
        }

        
    }
}
