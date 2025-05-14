# Cultivation Calculator Overview

This document outlines the calculation which would be performed by the bot to produce an estimate of how much longer it takes for a player to reach the next major cultivation stage.

This document will be divided into four sections, each focuses on a crucial source of cultivation exp income:
1. [Cultivation™ and Aura Gem](#cultivation-and-aura-gem)
2. [Cultivation Pills](#cultivation-pills)
3. [Myrimon Fruits and the Aura Extractor](#myrimon-fruits-and-the-aura-extractor)
4. [Creation Aftifacts: the Vase and the Mirror](#creation-artifacts-the-vase-and-the-mirror)

## Cultivation™ and Aura Gem

Cultivation™ and aura gem are players' passive cultivation exp income sources, with the aura gem being dependent on cultivation speed, and thus are discussed as a whole in this section. 

The calculation is straight forward:

$$ Exp = AbodeAura \times (PlayerAbsorptionRate + AuraGemAbsorptionRate) $$

where:

$$ AuraGemAbsorptionRate = PlayerAbsorptionRate * AuraGemMultiplier $$

Where $AuraGemMultiplier$ depends on the quality of the aura gem and is in the range of $[0.1, 0.28]$.

## Cultivation Pills

Todo!

## Myrimon Fruits and the Aura Extractor

Todo!

## Creation Artifacts: the Vase and the Mirror

Todo!