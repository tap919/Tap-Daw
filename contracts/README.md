# ProofOfAction Smart Contract

A Solidity smart contract for attesting actions with phase tracking (learn/ship/rest).

## Overview

The ProofOfAction contract allows users to attest their actions on-chain while tracking the phase they are in. This enables transparent tracking of activities across different stages of development or learning.

## Features

- **Phase Tracking**: Support for three phases - LEARN, SHIP, and REST
- **Event Emission**: All attestations emit events with phase information
- **Attestation Counting**: Track the number of attestations per address
- **Gas Efficient**: Optimized for minimal gas consumption

## Contract Interface

### Enums

```solidity
enum Phase {
    LEARN,  // 0 - Learning phase
    SHIP,   // 1 - Shipping/Building phase
    REST    // 2 - Resting phase
}
```

### Functions

#### attest(string memory action, Phase phase)

Attests an action with a specific phase.

**Parameters:**
- `action`: Description of the action being attested (cannot be empty)
- `phase`: The current phase (LEARN=0, SHIP=1, REST=2)

**Emits:** `Attested` event

**Example:**
```solidity
proofOfAction.attest("Completed TypeScript tutorial", Phase.LEARN);
proofOfAction.attest("Deployed new feature", Phase.SHIP);
proofOfAction.attest("Taking a break", Phase.REST);
```

#### getAttestationCount(address actor)

Returns the total number of attestations for a given address.

**Parameters:**
- `actor`: The address to query

**Returns:** `uint256` - The attestation count

### Events

#### Attested

Emitted when an action is attested.

```solidity
event Attested(
    address indexed actor,
    string action,
    Phase phase,
    uint256 timestamp
);
```

**Parameters:**
- `actor`: Address that made the attestation
- `action`: Description of the attested action
- `phase`: The phase during which the action was attested
- `timestamp`: Block timestamp of the attestation

## Installation

```bash
npm install
```

## Testing

Run the test suite:

```bash
npm test
```

## Compilation

Compile the smart contract:

```bash
npm run compile
```

## Deployment

The contract can be deployed to any EVM-compatible blockchain. Example deployment script:

```javascript
const { ethers } = require("hardhat");

async function main() {
  const ProofOfAction = await ethers.getContractFactory("ProofOfAction");
  const proofOfAction = await ProofOfAction.deploy();
  await proofOfAction.deployed();
  
  console.log("ProofOfAction deployed to:", proofOfAction.address);
}

main();
```

## Security Considerations

- Actions must not be empty strings
- Each attestation increments the counter for the sender
- All attestations are permanent and cannot be deleted
- Events can be monitored off-chain for analytics

## License

MIT
