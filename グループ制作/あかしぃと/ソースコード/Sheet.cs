using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sheet : MonoBehaviour
{
    Renderer sheet;
    RedSheet redSheet;
    GreenSheet greenSheet;

    void Start()
    {
        sheet = gameObject.GetComponent<Renderer>();

        {
            GameObject obj = GameObject.Find("RedSheet");
            redSheet = obj.GetComponent<RedSheet>();
        }

        {
            GameObject obj = GameObject.Find("GreenSheet");
            greenSheet = obj.GetComponent<GreenSheet>();
        }
    }

    void Update()
    {
        Color color = Color.clear;

        if (redSheet.RotEnd)
        {
            color = Color.red;
            color.a = .5f;
        }
        else
        {
            color.a = .0f;
        }

        if (!redSheet.RotEnd)
        {
            if (greenSheet.RotEnd)
            {
                color = Color.green;
                color.a = .5f;
            }
            else
            {
                color.a = .0f;
            }
        }

        sheet.material.color = color;
    }
}