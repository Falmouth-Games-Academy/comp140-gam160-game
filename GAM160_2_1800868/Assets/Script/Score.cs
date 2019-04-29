using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Score : MonoBehaviour {

    private float score;

    public Text scoreText;

	// Use this for initialization
	void Start () {
        //At the start the player score will be displayed
        scoreText.text = PlayerPrefs.GetFloat("Score", score).ToString();
	}
}
