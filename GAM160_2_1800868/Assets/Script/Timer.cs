using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using UnityEngine.SceneManagement;

public class Timer : MonoBehaviour {
    
    public float timer;
    [SerializeField]
    private Text timerTexts;
    [SerializeField]
    private ChangeScene cs;
    [SerializeField]
    private ScoreBoard scoreBoards;
    [SerializeField]
    private Text warning;


    // Use this for initialization
    void Start () {
        warning.enabled = false;
    }
	
	// Update is called once per frame
	void Update () {

        timer -= Time.deltaTime;
        timerTexts.text = Convert.ToInt16(timer).ToString();

        //When the timer hits zero it will save the players score and switch to the set scene
        if (timer <= 0)
        {
            if (cs != null && scoreBoards != null)
            {

                PlayerPrefs.SetFloat("Score", scoreBoards.score);
                cs.GetComponent<ChangeScene>().SwitchtoScene();
            }
        }
    }

    //The time reduction warning will appear when for 0.5 seconds
    public IEnumerator Warning()
    {
        warning.enabled = true;
        yield return new WaitForSeconds(0.5f);
        warning.enabled = false;
    }
}
