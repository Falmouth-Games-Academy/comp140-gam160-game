using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreBoard : MonoBehaviour {

    private Randomizer Values;
    [SerializeField]
    private Text scoreText;
    [SerializeField]
    private CheckScript drink;
    [SerializeField]
    private Shakermeter sm;
    private int count;
    public int score;
    public bool drinkAccounted;

    // Use this for initialization
    void Start () {
        
	}
	
	// Update is called once per frame
	void Update () {

        //When the glass if filled up it will reset the ingrediants list and shaker meter
        if (count >= 5)
        {
            count = 0;
            drink.i = 0;
            drink.resetList();
            score++;
            scoreText.text = score.ToString();
            drinkAccounted = false;
            sm.GetComponent<Shakermeter>().runOnce = false;
            sm.GetComponent<Shakermeter>().isShaken = false;

            if (Values != null)
                Values.RandomLayers();
        }

        
	}

    //Detects the 
    private void OnTriggerEnter(Collider other)
    {
        Destroy(other.gameObject);
        drinkAccounted = true;
        count++;
    }
}
