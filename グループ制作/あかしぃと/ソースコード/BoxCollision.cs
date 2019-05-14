using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoxCollision : MonoBehaviour
{
    bool hitBlock;
    bool hitBlockCollision;

    void Start()
    {
        hitBlock = false;
        hitBlockCollision = false;
    }

    void OnTriggerStay(Collider hit)
    {
        if (hit.gameObject.CompareTag("Block"))
        {
            hitBlock = true;
        }
        if (hit.gameObject.CompareTag("BlockCollision"))
        {
            hitBlockCollision = true;
        }
    }
    void OnTriggerExit(Collider hit)
    {
        if (hit.gameObject.CompareTag("Block"))
        {
            hitBlock = false;
        }
        if (hit.gameObject.CompareTag("BlockCollision"))
        {
            hitBlockCollision = false;
        }
    }

    public bool HitBlock
    {
        get { return hitBlock; }
    }

    public bool HitBlockCollision
    {
        get { return hitBlockCollision; }
    }
}