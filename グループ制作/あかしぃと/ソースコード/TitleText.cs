using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleText : MonoBehaviour {

    float ChangeAlpha = 1.0f;
    bool down = true;
    int count = 0;

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        this.GetComponent<SpriteRenderer>().color = new Color(1.0f, 1.0f, 1.0f, ChangeAlpha);

        if (down == true && count % 5 == 0)
        {
            ChangeAlpha -= 0.1f;

            if(ChangeAlpha < 0.0f)
            {
                ChangeAlpha = 0.0f;
                down = false;
            }
        }
        else if(down == false && count % 5 == 0)
        {
            ChangeAlpha += 0.1f;

            if(ChangeAlpha >= 1.0f)
            {
                ChangeAlpha = 1.0f;
                down = true;
            }
        }

        count++;
	}
}
