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



    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        timer -= Time.deltaTime;
        timerTexts.text = Convert.ToInt16(timer).ToString();

        if (timer <= 0)
        {
            if (cs != null)
            {
                cs.GetComponent<ChangeScene>().SwitchtoScene();
            }
        }
    }
}
