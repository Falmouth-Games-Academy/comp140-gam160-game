using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CheckScript : MonoBehaviour {

    [SerializeField]
    private Randomizer values;
    [SerializeField]
    private ScoreBoard sb;
    [SerializeField]
    private Arduino arduino;
    [SerializeField]
    private LidScript ls;
    [SerializeField]
    private float gadgeLimit;

    public Timer timer;
    public bool isMade;

    private List<int> ingrediantValues;
    public int i = 0;
    private int numOfLayers = 0;
    private float gadge;
    private float wrongIngrediantGadge;
    private bool isFull;

	// Use this for initialization
	void Start () {
        wrongIngrediantGadge = 0;
    }

    // Update is called once per frame
    void Update () {

        //If the shaker detects the wrong ingreiant then the player's time will be reduced and the warning from the timer script will appear
        if (wrongIngrediantGadge >= 10)
        {
            timer.timer -= 5;
            timer.StartCoroutine(timer.Warning());
            wrongIngrediantGadge = 0;
        }

        if (i >= 5)
        {
            isMade = true;
        }
        else
            isMade = false;

        //Debug.Log("checkscript: " + isMade);

        /*if (!isFull)
            isMade = false;
            */
            
	}

    
    private void OnTriggerEnter(Collider other)
    {
        if (other.tag != "SpawnPoint")
        {
            Destroy(other.gameObject);
        }

        //if the shaker lid is open and if the ingrediant matches the current needed ingrediant
        //then it will state that the ingrediant has been added and go on to the next one
        if (ls.lidIsOpen)
        {
            isFull = false;
            if (values.layerNums[i].ToString() == other.transform.tag)
            {
                gadge++;
                if (gadge > gadgeLimit)
                {

                    i = i + 1;
                    gadge = 0;
                    values.Layers[i-1].color = Color.green;
                }
            }
            else
            {
                wrongIngrediantGadge++;
            }
        }
        else if (numOfLayers >= 5)
        {
            isFull = true;
        }
    }

    //Each layer in the list will be set back to white
    public void resetList()
    {
        for (int i = 0; i < 5; i++)
        {
            values.Layers[i].color = Color.white;
        }
    }
}
