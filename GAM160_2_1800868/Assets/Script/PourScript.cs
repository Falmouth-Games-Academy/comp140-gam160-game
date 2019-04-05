using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PourScript : MonoBehaviour {

    [SerializeField]
    private Transform spawnPoint;
    [SerializeField]
    private GameObject drink;
    [SerializeField]
    private CheckScript cs;
    // Use this for initialization
    void Start () {
        cs = GetComponent<CheckScript>();
	}
	
	// Update is called once per frame
	void Update () {

        if (transform.eulerAngles.z < 220 && transform.eulerAngles.z > 150)
        {
            if (cs != null)
            {
                Debug.Log("pouring Script: " + cs.isMade);
                if (Input.GetMouseButtonDown(1) && cs.isMade)
                {
                    Instantiate(drink, spawnPoint.position, Quaternion.identity);
                }
            }
        }

        if (Input.GetMouseButton(0))
        {
            transform.eulerAngles = new Vector3(0,0,180);
        }
        else if (Input.GetMouseButtonUp(0))
        {
            transform.eulerAngles = new Vector3(0, 0, 0);
        }
    }
}
