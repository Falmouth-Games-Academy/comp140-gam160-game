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
    private int count;
    private int score;
    public bool drinkAccounted;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (count >= 5)
        {
            count = 0;
            drink.i = 0;
            drink.resetList();
            score++;
            scoreText.text = score.ToString();
            drinkAccounted = false;
            Values.RandomLayers();
        }

        
	}

    private void OnTriggerEnter(Collider other)
    {
        Destroy(other.gameObject);
        drinkAccounted = true;
        count++;

    }
}
