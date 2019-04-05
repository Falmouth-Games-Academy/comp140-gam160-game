using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Randomizer : MonoBehaviour {
    
    [SerializeField]
    private Image Layer;
    [SerializeField]
    private Transform layerBackground;
    [SerializeField]
    public List<Image> Layers = new List<Image>();
    public List<int> layerNums = new List<int>();

    DrinkClass ingrediantLayer = new DrinkClass();
    string[] ingrediantsList = new string[] { "Sugar Syrup", "Lemon Juice", "Lime Juice", "Dry Gin", "Vodka"};
    int randomNum;

    // Use this for initialization
    void Start () {
        RandomLayers();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void RandomLayers()
    {
        foreach (Image layer in Layers)
        {
            string String = ingrediantsList[0].ToString();
            randomNum = Random.Range(0, ingrediantsList.Length);

            layer.GetComponentInChildren<Text>().text = ingrediantsList[randomNum];
            layerNums.Add(randomNum);
        }
    }
}
