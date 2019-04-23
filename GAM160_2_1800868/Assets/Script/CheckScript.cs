using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckScript : MonoBehaviour {

    [SerializeField]
    private Randomizer values;
    [SerializeField]
    private ScoreBoard sb;
    [SerializeField]
    private Arduino arduino;

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
        if (wrongIngrediantGadge >= 10)
        {
            timer.timer -= 5;
            wrongIngrediantGadge = 0;
        }

        if (i >= 5)
        {
            isMade = true;
        }
        else
            isMade = false;

        Debug.Log("checkscript: " + isMade);

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

        if (numOfLayers < 5)
        {
            isFull = false;
            if (values.layerNums[i].ToString() == other.transform.tag)
            {
                gadge++;
                arduino.WriteToArduio("q");
                if (gadge > 3)
                {

                    i = i + 1;
                    gadge = 0;
                    values.Layers[i-1].color = Color.green;
                }
            }
            else
            {
                Debug.Log("Wrong ingrediant");
                wrongIngrediantGadge++;
            }
        }
        else if (numOfLayers >= 5)
        {
            isFull = true;
        }
    }

    public void resetList()
    {
        for (int i = 0; i < 5; i++)
        {
            values.Layers[i].color = Color.white;
        }
    }
}
