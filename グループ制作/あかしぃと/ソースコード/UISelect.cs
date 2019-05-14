using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UISelect : MonoBehaviour {

    public GameObject[] Controller;
    public GameObject[] Keyboard;

    string[] vs;


    // Use this for initialization
    void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
         vs = Input.GetJoystickNames();

        if (vs.Length <= 0)
        {
            Debug.Log("ControllerFalse");

            for (int i = 0; i < 5; i++)
            {
                Controller[i].SetActive(false);

                Keyboard[i].SetActive(true);
            }     
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                Controller[i].SetActive(true);

                Keyboard[i].SetActive(false);
            }
        }


    }
}
